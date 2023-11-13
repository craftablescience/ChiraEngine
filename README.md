<div align="center">
  <img width="400px" src="https://github.com/craftablescience/ChiraEngine/blob/main/branding/github/readme_banner.png?raw=true" alt="Chira Engine"/>

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5c8b0a7eba614cc8bc44d7d54789cf51)](https://app.codacy.com/gh/craftablescience/ChiraEngine?utm_source=github.com&utm_medium=referral&utm_content=craftablescience/ChiraEngine&utm_campaign=Badge_Grade_Settings)
![Workflow Status](https://img.shields.io/github/actions/workflow/status/craftablescience/ChiraEngine/build_engine.yml?branch=main&label=builds%20and%20tests)
![License](https://img.shields.io/github/license/craftablescience/ChiraEngine?label=license)
![Discord](https://img.shields.io/discord/678074864346857482?label=discord&logo=Discord&logoColor=%23FFFFFF)

A fully customizable (and heavily work-in-progress) MIT-licensed game engine, written in C++20.
</div>

## Features

To summarize the major features:
- Native Windows, Linux, and macOS support
- As extensible as possible: register custom logging hooks, resource types and providers, etc.
- Custom preprocessing of GLSL shaders, with macros and #include directives
- Cached resource loading and management
- Data-oriented design, using an ECS framework
- Supports multiple simultaneous windows
- Hassle-free Discord Rich Presence and Steamworks API wrappers
- Inline text localization
- Console commands to run actions on the fly, with the ability to pass arguments
- Variables modifiable from the console, with support for caching between sessions
- Logger built-in with colored text and hooks
- Multipurpose game editor

The goal of the engine is to have as much customization as possible, while still maintaining ease of use.

## Supported Platforms

- Windows 7+
- Linux
- macOS 11.0+

### Planned:

- Android
- Web (HTML5)

## Supported Compilers

Compilers in bold are prioritized.

| Platform | **GCC** | Clang | **MSVC** |
|----------|:-------:|:-----:|:--------:|
| Windows  |   ✔\*   |  ✔\*  |    ✔     |
| Linux    |    ✔    |   ✔   |    -     |
| macOS    |    ✖    | ✔\*\* |    -     |

\* Supported with MinGW. Packaging a build of the application will require you to copy a DLL
from the MinGW bin directory next to the executable. Check the GitHub Actions script to see the path to the DLL.

\*\* Homebrew LLVM/Clang is required. Some C++ features used in this project are not supported
by Apple's version of Clang.

## Supported Graphics APIs

| Platform          | OpenGL 4.0 | OpenGL 4.1 | OpenGL 4.3 | Direct3D 11 | Software |
|-------------------|:----------:|:----------:|:----------:|:-----------:|:--------:|
| Windows           |     ✔      |     ✔      |     ✔      |   Planned   |   ✔\*    |
| Linux             |     ✔      |     ✔      |     ✔      |      -      |   ✔\*    |
| macOS             |     ✔      |     ✔      |     -      |      -      |   ✔\*    |

\* SDL software renderer implementation is incomplete and not recommended for 3D.
It exists to make it easier to port to new platforms.

## Bundled Dependencies

- Docs:
  - [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)
- Engine:
  - [cereal](https://github.com/craftablescience/cereal) v1.3.2
  - [Dear ImGui](https://github.com/ocornut/imgui) v1.89.5
  - [Discord RPC](https://github.com/craftablescience/discord-rpc-clean)
  - [entt](https://github.com/skypjack/entt) v3.11.1
  - [{fmt}](https://github.com/fmtlib/fmt) v9.1.0
  - [glad](https://gen.glad.sh/) v2.0.4
  - [glm](https://github.com/g-truc/glm) v0.9.9
  - [ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit)
  - [ImGui Filebrowser](https://github.com/AirGuanZ/imgui-filebrowser)
  - [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) v1.89
  - [LibLoader](https://github.com/craftablescience/LibLoader)
  - [lua](https://github.com/craftablescience/lua) v5.4.6
  - [magic_enum](https://github.com/Neargye/magic_enum) v0.8.2
  - [nlohmann_json](https://github.com/nlohmann/json) v3.11.2
  - [physfs](https://github.com/craftablescience/physfs) v3.2.0
  - [sdl](https://github.com/libsdl-org/SDL) v2.26.5
  - [sol2](https://github.com/ThePhD/sol2) v3.3.0
  - [soloud](https://github.com/craftablescience/soloud)
  - [stb_image](https://github.com/nothings/stb) v2.27
  - [stduuid](https://github.com/mariusbancila/stduuid) v1.1

## Development (Windows)

- **CLion**: The project will compile without any prior configuration as long as a compiler is installed,
  but you will need to install the Windows SDK from the Visual Studio Installer.

- **Visual Studio 2022**: You will need to install the Windows SDK (any version should work) and the CMake
  integration component. If you do not have any other compilers, you will need MSVC as well.

- If compiling with WSL without using any IDE integrations, skip to the Linux section.

## Development (Linux)

The project will compile without any prior configuration in your IDE of choice (or the terminal),
but you will need to install a few things first. On Debian-based distros, run:

```shell
sudo apt update && sudo apt install cmake build-essential xorg-dev mesa-common-dev mesa-utils
```

Installing Ninja is optional but recommended:

```shell
sudo apt install ninja
```

You will also need to install dev versions of sound libraries to support each of these sound backends when compiling.
If you are compiling the project for your personal use, you only have to install the one you need:

```shell
sudo apt install libasound2-dev libpulse-dev libjack-dev
```

If you are not using an IDE, the project can be compiled using the following commands:

```shell
cmake -G "Unix Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug -- -j$(nproc)
cmake --build build
```

- Replace `"Unix Makefiles"` with `"Ninja"` if you installed Ninja earlier.
- Replace `Debug` with `Release` if you want to compile a release build.

## Development (macOS)

You must install LLVM from Homebrew, as the default Apple Clang compilers do not work properly:

```shell
brew install llvm
```

Installing Ninja is optional but recommended:

```shell
brew install ninja
```

If you are not using an IDE, the project can be compiled using the following commands:

```shell
cmake -G "Unix Makefiles" -B build -DCMAKE_C_COMPILER="/usr/local/opt/llvm/bin/clang" -DCMAKE_CXX_COMPILER="/usr/local/opt/llvm/bin/clang++" -DCMAKE_BUILD_TYPE=Debug -- -j$(sysctl -n hw.logicalcpu)
cmake --build build
```

- Replace `"Unix Makefiles"` with `"Ninja"` if you installed Ninja earlier.
- Replace `Debug` with `Release` if you want to compile a release build.
