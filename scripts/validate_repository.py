from __future__ import annotations

import hashlib
import io
import re
import stat
import sys
import zipfile
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import unquote, urlsplit


ROOT = Path(__file__).resolve().parents[1]
TEMPLATE = ROOT / "template"
DOCS = ROOT / "docs"
VERSION = "1.0.0"
GUIDE_VERSION = "3.2"
TEMPLATE_ASSET = f"VEXLearn.Template.V{VERSION}.zip"
GUIDE_ASSET = f"VEXLearn.Guide.V{GUIDE_VERSION}.pdf"
PUBLIC_MOVEMENT_APIS = {"move", "turn"}
CURRICULUM_FILES = {
    "pre-assessment.pdf",
    "post-assessment.pdf",
    "module-1-debugging-exercise.pdf",
    "module-2-project-map.pdf",
    "module-3-configuration-planner.pdf",
    "module-4-route-planner.pdf",
    "module-5-pid-lab.pdf",
    "module-6-reliability-log.pdf",
    "capstone-rubric.pdf",
    "instructor-guide.pdf",
}

REQUIRED_TEMPLATE_FILES = {
    ".gitignore",
    ".vscode/c_cpp_properties.json",
    ".vscode/extensions.json",
    ".vscode/settings.json",
    ".vscode/vex_project_settings.json",
    "include/auto.h",
    "include/display.h",
    "include/drive.h",
    "include/movement.h",
    "include/robotConfig.h",
    "include/vex.h",
    "makefile",
    "src/auto.cpp",
    "src/display.cpp",
    "src/drive.cpp",
    "src/main.cpp",
    "src/movement.cpp",
    "src/robotConfig.cpp",
    "vex/mkenv.mk",
    "vex/mkrules.mk",
}
VOID_TAGS = {
    "area",
    "base",
    "br",
    "col",
    "embed",
    "hr",
    "img",
    "input",
    "link",
    "meta",
    "param",
    "source",
    "track",
    "wbr",
}
GENERATED_DIRECTORIES = {"bin", "build"}
GENERATED_FILES = {"compile_commands.json", "compile_flags.txt"}
GENERATED_SUFFIXES = {".d", ".elf", ".hex", ".o"}


class PageParser(HTMLParser):
    def __init__(self, name: str) -> None:
        super().__init__(convert_charrefs=True)
        self.name = name
        self.ids: list[str] = []
        self.links: list[str] = []
        self.stack: list[tuple[str, int]] = []
        self.errors: list[str] = []

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        values = {key: value or "" for key, value in attrs}
        if values.get("id"):
            self.ids.append(values["id"])
        for attribute in ("href", "src"):
            if values.get(attribute):
                self.links.append(values[attribute])
        if tag not in VOID_TAGS:
            self.stack.append((tag, self.getpos()[0]))

    def handle_startendtag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        self.handle_starttag(tag, attrs)
        if tag not in VOID_TAGS:
            self.stack.pop()

    def handle_endtag(self, tag: str) -> None:
        if not self.stack:
            self.errors.append(f"{self.name}:{self.getpos()[0]} unexpected </{tag}>")
            return
        open_tag, line = self.stack.pop()
        if open_tag != tag:
            self.errors.append(
                f"{self.name}:{self.getpos()[0]} closes <{open_tag}> from line {line} with </{tag}>"
            )

    def close(self) -> None:
        super().close()
        for tag, line in reversed(self.stack):
            self.errors.append(f"{self.name}:{line} unclosed <{tag}>")


def require(condition: bool, message: str, errors: list[str]) -> None:
    if not condition:
        errors.append(message)


def validate_template(errors: list[str]) -> None:
    files = {
        path.relative_to(TEMPLATE).as_posix()
        for path in TEMPLATE.rglob("*")
        if path.is_file()
    }
    missing = sorted(REQUIRED_TEMPLATE_FILES - files)
    require(not missing, f"Missing template files: {', '.join(missing)}", errors)

    for path in ROOT.rglob("*"):
        if not path.is_file() or ".git" in path.parts or "dist" in path.parts:
            continue
        relative = path.relative_to(ROOT)
        parts = {part.lower() for part in relative.parts}
        if (
            parts & GENERATED_DIRECTORIES
            or path.name.lower() in GENERATED_FILES
            or path.suffix.lower() in GENERATED_SUFFIXES
        ):
            errors.append(f"Generated artifact is tracked or present: {relative.as_posix()}")


def validate_html(errors: list[str]) -> None:
    pages: dict[Path, PageParser] = {}
    for path in sorted(DOCS.glob("*.html")):
        parser = PageParser(path.name)
        parser.feed(path.read_text(encoding="utf-8"))
        parser.close()
        pages[path] = parser
        errors.extend(parser.errors)
        duplicates = sorted({item for item in parser.ids if parser.ids.count(item) > 1})
        require(not duplicates, f"Duplicate IDs in {path.name}: {', '.join(duplicates)}", errors)

    for page, parser in pages.items():
        for raw_url in parser.links:
            split = urlsplit(raw_url)
            if split.scheme in {"data", "http", "https", "mailto", "tel"} or raw_url.startswith("//"):
                continue
            url_path = unquote(split.path)
            target = (DOCS / url_path.lstrip("/")) if url_path.startswith("/") else (page.parent / url_path)
            if url_path:
                require(target.exists(), f"Broken local link in {page.name}: {raw_url}", errors)
            target_page = target if target.suffix.lower() == ".html" else page if not url_path else None
            if split.fragment and target_page in pages:
                require(
                    split.fragment in pages[target_page].ids,
                    f"Missing fragment target in {page.name}: {raw_url}",
                    errors,
                )

    css = (DOCS / "styles.css").read_text(encoding="utf-8")
    for raw_url in re.findall(r"url\(([^)]+)\)", css):
        value = raw_url.strip(" \"'")
        if value.startswith(("data:", "http://", "https://")):
            continue
        require((DOCS / value).exists(), f"Broken CSS asset: {value}", errors)

    curriculum = DOCS / "curriculum.html"
    require(curriculum in pages, "Curriculum page is missing", errors)
    if curriculum in pages:
        source = curriculum.read_text(encoding="utf-8")
        require('data-storage-key="vexlearn-foundations-progress-v1"' in source, "Curriculum progress storage is missing", errors)
        expected_ids = {f"module-{number}" for number in range(7)} | {"capstone"}
        check_ids = set(re.findall(r'data-check-id="([^"]+)"', source))
        require(check_ids == expected_ids, f"Unexpected curriculum progress IDs: {sorted(check_ids)}", errors)

    curriculum_files = DOCS / "curriculum"
    missing_files = sorted(name for name in CURRICULUM_FILES if not (curriculum_files / name).is_file())
    require(not missing_files, f"Missing curriculum PDFs: {', '.join(missing_files)}", errors)
    empty_files = sorted(name for name in CURRICULUM_FILES if (curriculum_files / name).is_file() and (curriculum_files / name).stat().st_size == 0)
    require(not empty_files, f"Empty curriculum PDFs: {', '.join(empty_files)}", errors)


def validate_versions_and_apis(errors: list[str]) -> None:
    readme = (ROOT / "README.md").read_text(encoding="utf-8")
    changelog = (ROOT / "CHANGELOG.md").read_text(encoding="utf-8")
    website = "\n".join(
        path.read_text(encoding="utf-8")
        for path in DOCS.rglob("*")
        if path.is_file() and path.suffix.lower() in {".css", ".html", ".js"}
    )
    template_text = "\n".join(
        path.read_text(encoding="utf-8", errors="replace")
        for path in TEMPLATE.rglob("*")
        if path.is_file()
    )
    current_surfaces = "\n".join((readme, website))
    source_and_documentation = "\n".join((current_surfaces, template_text))

    require(f"Version {VERSION} is the current stable release" in readme, "README stable version is outdated", errors)
    require(f"## [{VERSION}] - 2026-08-06" in changelog, "Changelog release date or version is outdated", errors)
    require(f"v{VERSION}</span>" in website, "Website version badge is outdated", errors)
    require(f"Software Version: {VERSION}" in template_text, "Template display version is outdated", errors)
    require("include_prereleases" not in readme, "README release badge includes prereleases", errors)
    require("release candidate" not in "\n".join((readme, changelog, website)).lower(), "Release-candidate wording remains", errors)
    outdated_versions = sorted(set(re.findall(r"\bv?0\.\d+\.\d+\b", current_surfaces, re.IGNORECASE)))
    require(not outdated_versions, f"Outdated template versions remain: {', '.join(outdated_versions)}", errors)
    require(TEMPLATE_ASSET == "VEXLearn.Template.V1.0.0.zip", "Template release filename is outdated", errors)
    require(GUIDE_ASSET == "VEXLearn.Guide.V3.2.pdf", "Guide release filename is outdated", errors)

    header = (TEMPLATE / "include" / "movement.h").read_text(encoding="utf-8")
    implementation = (TEMPLATE / "src" / "movement.cpp").read_text(encoding="utf-8")
    public_names = set(re.findall(r"^void\s+(\w+)\s*\(", header, re.MULTILINE))
    require(public_names == PUBLIC_MOVEMENT_APIS, f"Unexpected public movement APIs: {sorted(public_names)}", errors)
    for name in public_names:
        require(re.search(rf"^void\s+{name}\s*\(", implementation, re.MULTILINE) is not None, f"Missing {name} definition", errors)
        require(f"`{name}()`" in readme and f"{name}()" in website, f"Documentation is missing {name}()", errors)
    require(not re.search(r"\b(?:moveRPM|turnRPM)\b", source_and_documentation), "Removed RPM API remains", errors)


def package_template() -> tuple[Path, str]:
    def build() -> bytes:
        output = io.BytesIO()
        with zipfile.ZipFile(output, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
            for path in sorted(item for item in TEMPLATE.rglob("*") if item.is_file()):
                relative = path.relative_to(TEMPLATE).as_posix()
                info = zipfile.ZipInfo(f"VEXLearn Template/{relative}", (1980, 1, 1, 0, 0, 0))
                info.compress_type = zipfile.ZIP_DEFLATED
                # Fixed timestamps and permissions make identical source produce identical ZIP bytes on every runner.
                info.create_system = 3
                info.external_attr = (stat.S_IFREG | 0o644) << 16
                archive.writestr(info, path.read_bytes(), compress_type=zipfile.ZIP_DEFLATED, compresslevel=9)
        return output.getvalue()

    first = build()
    second = build()
    if first != second:
        raise RuntimeError("Template packaging is not reproducible")
    destination = ROOT / "dist" / TEMPLATE_ASSET
    destination.parent.mkdir(exist_ok=True)
    destination.write_bytes(first)
    return destination, hashlib.sha256(first).hexdigest()


def main() -> int:
    errors: list[str] = []
    validate_template(errors)
    validate_html(errors)
    validate_versions_and_apis(errors)
    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1

    package, digest = package_template()
    print(f"Validated repository and created {package.relative_to(ROOT).as_posix()}")
    print(f"SHA256: {digest}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
