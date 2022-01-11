# Chira Engine
![Windows Workflow Status](https://img.shields.io/github/workflow/status/craftablescience/ChiraEngine/Build%20(Windows)?label=Windows)
![Linux Workflow Status](https://img.shields.io/github/workflow/status/craftablescience/ChiraEngine/Build%20(Linux)?label=Linux)
![License](https://img.shields.io/github/license/craftablescience/ChiraEngine?label=License)
![Discord](https://img.shields.io/discord/678074864346857482?label=Discord&logo=Discord&logoColor=%23FFFFFF)

A fully customizable (and heavily work-in-progress) MIT-licensed game engine, written in C++17.

## Features
To summarize the major features:
- Native Windows and Linux support (OpenGL 4.5 Core)
- As extensible as possible: register custom logging hooks, sound providers, resource types and providers, etc.
- Custom preprocessing of GLSL shaders, with macros and #include directives
- Robust resource loading and management to facilitate sharing of congruent data
- Data-oriented design, using entities as building blocks
- Hassle-free Discord Rich Presence
- Inline text localization
- AngelScript support, with common function hooks and types registered
- Bullet physics engine built-in, with common physics types registered
- Event queue to communicate between entities (or other parts of the engine)
- Settings manager, which saves to and loads from a JSON file (with smart overwriting) for modifiable settings without recompiling
- Sound manager can be changed at runtime
- Logger built-in with colored text and hooks, and a console GUI window which uses the logging hook system

## Planned Features
- Create an editor UI to make games with
- Revamp scripting, allow adding custom scripting providers
- Add more graphics features
- Implement the assimp library for model loading
- Particle systems! Preferably offloaded to the GPU

Again, the goal of the engine is to have as much customization as possible, while still maintaining ease of use.

## Supported Compilers

|          | Windows                    | Linux            |
|----------|----------------------------|------------------|
| GCC      | Fully supported (on MinGW) | Fully supported  |
| Clang    | Fully supported (on MinGW) | Fully supported  |
| MSVC     | Partially supported*       | N/A              |
| Clang-CL | Untested                   | N/A              |

*MSVC cannot compile the engine's dependencies as
shared libraries, and can only compile for x86.

## Bundled Dependencies
- AngelScript v2.34.0
- Bullet v3.21
- cURL v7.77.0
- cURLpp v0.8.1
- Discord RPC
- {fmt} v8.1.1
- GLAD (OpenGL 4.5 Core, no extensions)
- GLFW v3.3.7
- GLM v0.9.9.9
- Dear ImGui v1.86 (docking)
- imgui_markdown
- nlohmann_json v3.10.5
- libogg v1.3.5
- libvorbis v1.3.7
- OpenAL Soft v1.21.1
- stb_image v2.27
- stduuid v1.1
- TinyFileDialogs v3.8.8

## Development (Windows)
- **CLion (recommended)**: The project will compile without any prior configuration, but you will most likely need to install the Windows SDK (see below).

- Visual Studio 2019: You will need to install (most of) the following components. Some are not required, like the AddressSanitizer and Just-In-Time debugger.
  ![image](https://user-images.githubusercontent.com/26600014/128105644-cfa92f30-dc96-4476-a4c9-8d8b5f3ce129.png)
  
  Compiling with MSVC will produce an error for x64 targets, due to a bug with AngelScript (see [the AngelScript docs](https://www.angelcode.com/angelscript/sdk/docs/manual/doc_compile_lib.html#doc_compile_win64) for more information).
  It will also build the engine and its dependencies statically.
  I strongly recommend compiling with MinGW, but if you can't, target x86.

## Development (Linux)
- **CLion (recommended)**: The project will compile without any prior configuration, but you will need to install a few things first.
  
  On Debian-based distros, run:
  - `sudo apt update && sudo apt install cmake build-essential xorg-dev mesa-common-dev mesa-utils libasound2-dev libpulse-dev libjack-dev curl libcurl4-gnutls-dev`
  - You can remove the audio libraries you don't use from this command, but keep in mind this will affect the end users of your project.

## Usage
I recommend adding this repository as a submodule to your project.
Make sure if you add a new resource folder, copy it to the `resources` folder in the executable directory in your buildscript, or the program will fail at runtime due to missing assets.

A good example buildscript and application can be found at:

[https://github.com/craftablescience/ChiraEditor](https://github.com/craftablescience/ChiraEditor)
