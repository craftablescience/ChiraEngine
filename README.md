# Chira Engine
![Discord](https://img.shields.io/discord/678074864346857482?label=Discord&logo=Discord&logoColor=%23FFFFFF&style=flat-square)

A fully customizable (and heavily work-in-progress) game engine written in C++17.

## Features
To summarize the major features:
- Native Windows and Linux support (OpenGL 4.5 Core)
- As extensible as possible: register custom logging hooks, scripting providers, sound providers, etc.
- Robust resource loading and management, and custom resource providers can be registered
- Hassle-free Discord Rich Presence
- Inline text localization
- AngelScript support, with common function hooks and types registered
- Bullet physics engine built-in
- Abstract input system using C++11 lambdas (or a reference to a function)
- Settings manager, which saves to and loads from a JSON file (with smart overwriting) for modifiable settings without recompiling
- Both AngelScript and OpenAL can be disabled in code and/or through the settings file
- The sound manager can be changed, and scripting providers (defined by the user) can be added
- Logger built-in with colored text and hooks, and a console GUI which uses the logging hook system

Again, the goal of the engine is to have as much customization as possible, while still maintaining ease of use.

## Bundled Dependencies
- AngelScript v2.34.0
- Bullet v3.17
- Discord RPC
- {fmt} v8.0.1
- GLAD (OpenGL 4.5 Core, no extensions)
- GLFW v3.3.5
- GLM v0.9.9.9
- Dear ImGui v1.84.2 (docking branch)
- imgui_markdown
- libogg v1.3.5
- libvorbis v1.3.7
- nlohmann_json v3.10.0
- OpenAL Soft v1.21.1
- stb_image v2.27

## Compilation
- **CLion (recommended)**: The project will compile without any prior configuration. If developing on Windows, you will need to install the Windows SDK (see below).

- Visual Studio 2019: You will need to install the following components:

![image](https://user-images.githubusercontent.com/26600014/128105644-cfa92f30-dc96-4476-a4c9-8d8b5f3ce129.png)

Additionally, compiling with MSVC will produce an error for x64 targets, due to a bug with AngelScript (see [the AngelScript docs](https://www.angelcode.com/angelscript/sdk/docs/manual/doc_compile_lib.html#doc_compile_win64) for more information).
I recommend compiling with MinGW if you can help it, or targeting x86 if you can't.

## Usage
I recommend adding this repository as a submodule to your project.
Make sure to copy the `resources` folder to the executable directory in your buildscript, or it will fail due to missing assets.

A good example buildscript and application can be found at:

[https://github.com/craftablescience/ChiraEditor](https://github.com/craftablescience/ChiraEditor)
