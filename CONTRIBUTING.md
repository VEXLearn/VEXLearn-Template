# Contributing to VEXLearn Template

Thank you for helping improve VEXLearn. Contributions should keep the project beginner-friendly, instructional, and compatible with the official VEX VS Code extension.

## Before You Start

- Search existing issues before opening a new one.
- Use a bug report for incorrect behavior and a documentation report for unclear or incorrect guidance.
- Keep changes focused. Do not add frameworks, dependencies, generated build files, or unrelated rewrites.

## Making a Change

1. Fork the repository and create a short, descriptive branch.
2. Match the existing C++ formatting, naming, and inline-comment style.
3. Explain non-obvious control logic beside the relevant code.
4. Update the website and guide when a public function or documented behavior changes.
5. Confirm that generated `build/`, `bin/`, ZIP, and PDF artifacts are not included in the commit.
6. Open a pull request using the repository template.

VEX compilation and physical-robot testing are expected for movement or hardware changes. If you cannot perform them, state that clearly in the pull request.

## Writing Style

- Write for readers who may be new to C++ and VEX.
- Prefer short, direct explanations and display-safe ASCII in robot-screen strings.
- Preserve public API names unless the change explicitly requires a breaking release.
- Keep examples consistent across source comments, the website, and the PDF guide.

By participating, you agree to follow the [Community Guidelines](COMMUNITY_GUIDELINES.md).
