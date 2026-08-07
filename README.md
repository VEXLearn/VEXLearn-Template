<div align="center">
  <picture>
    <img src="docs/images/vexlearn-icon.png" alt="VEXLearn logo" width="132">
  </picture>
  <h1>VEXLearn Template</h1>
  <p><strong>A beginner-friendly VEX V5 C++ template designed for learning through documented, editable robot code.</strong></p>
  <p>
    <img src="https://img.shields.io/badge/platform-VEX%20V5-d22630" alt="VEX V5">
    <img src="https://img.shields.io/badge/C%2B%2B-11-00599c" alt="C++11">
    <a href="https://github.com/VEXLearn/VEXLearn-Template/releases/latest"><img src="https://img.shields.io/github/v/release/VEXLearn/VEXLearn-Template" alt="Latest release"></a>
    <a href="https://github.com/VEXLearn/VEXLearn-Template/actions/workflows/validate.yml"><img src="https://github.com/VEXLearn/VEXLearn-Template/actions/workflows/validate.yml/badge.svg" alt="Repository validation"></a>
    <a href="LICENSE"><img src="https://img.shields.io/github/license/VEXLearn/VEXLearn-Template" alt="MIT License"></a>
    <a href="https://github.com/VEXLearn/VEXLearn-Template/releases">
  <img src="https://img.shields.io/github/downloads/VEXLearn/VEXLearn-Template/total?label=release%20downloads" alt="Release downloads">
</a>
  </p>
</div>



VEXLearn is a beginner-focused teaching template for tank-drive VEX V5 robots. Its source files include introductions, structure explanations, inline comments, and troubleshooting guidance so new programmers can learn from working code instead of a black-box library.

Version 1.0.0 is the current stable release on the `main` branch.

## Features

- Voltage-based `move()` and `turn()` PID functions with heading correction and integral windup prevention
- Tank-drive control with configurable deadband, sensitivity, and coast-to-brake timing
- Brain and controller displays for motor, battery, team, heading, and software information
- Example autonomous routes, mechanism controls, and piston toggles
- Companion website and VEXLearn Guide v3.2 for setup, C++ fundamentals, VEX concepts, and PID tuning

## Requirements

- A tank-drive VEX V5 robot with separate left and right motor groups
- VEX V5 motors, one controller, and an inertial sensor
- [Visual Studio Code](https://code.visualstudio.com/) with the official VEX Robotics extension
- Windows, macOS, or Linux supported by the VEX extension

PROS, VEX IQ, VEX EXP, X-drive, H-drive, and mecanum configurations are not supported by this template.

## Quick Start

1. Download the [latest release](https://github.com/VEXLearn/VEXLearn-Template/releases/latest).
2. Extract the ZIP and open the inner `VEXLearn Template` folder in Visual Studio Code.
3. Follow the [setup guide](https://vexlearn.github.io/VEXLearn-Template/setup.html) to configure motors, sensors, wheel size, and gear ratio.
4. Tune `move()` and `turn()` on your physical robot before writing competition routes.

Do not install Microsoft's Makefile Tools extension when prompted. The official VEX extension provides the required build environment.

## Repository Structure

```text
template/       Clean VEX V5 C++ project source
docs/           Static VEXLearn website served by GitHub Pages
.github/        Community templates and repository validation
CHANGELOG.md    Release history
```

Generated VEX build files and packaged release assets are intentionally excluded from the tracked source.

## Documentation

- [VEXLearn website](https://vexlearn.github.io/VEXLearn-Template/)
- [Setup guide](https://vexlearn.github.io/VEXLearn-Template/setup.html)
- [C++ fundamentals](https://vexlearn.github.io/VEXLearn-Template/cpp.html)
- [VEX concepts and PID](https://vexlearn.github.io/VEXLearn-Template/vex-concepts.html)
- [Latest guide and template release](https://github.com/VEXLearn/VEXLearn-Template/releases/latest)

## Contributing and Support

Read [CONTRIBUTING.md](CONTRIBUTING.md) before submitting a change. For bugs or documentation problems, use the appropriate [issue form](https://github.com/VEXLearn/VEXLearn-Template/issues/new/choose). Report security problems privately through the instructions in [SECURITY.md](SECURITY.md).

For competition-focused odometry and coordinate systems, consider [JAR Template](https://github.com/JacksonAreaRobotics/JAR-Template) or [LemLib](https://github.com/LemLib/LemLib).

## License

VEXLearn Template is released under the [MIT License](LICENSE).
