# Chira Engine
![Discord](https://img.shields.io/discord/678074864346857482?label=Discord&logo=Discord&logoColor=%23FFFFFF&style=flat-square)

A fully customizable (and heavily work-in-progress) game engine written in C++17.

## Features
There may be more that I have missed, but:
- Native Windows and Linux support (currently using OpenGL 3.3 core)
- As extensible as possible: register custom logging hooks, scripting providers, sound providers, etc.
- Hassle-free Discord Rich Presence
- AngelScript support, with common function hooks and types registered
- Hassle-free input system using C++11 lambdas (or a reference to a function, your choice)
- Loads OBJ mesh files
- Easy to use settings manager, which saves to and loads from a JSON file (with smart overwriting) for modifiable settings without recompiling
- Plays OGG sound files
- Both AngelScript and OpenAL can be disabled in code or in the settings file
- The sound manager can be changed, and several scripting providers (defined by the user) can be added at once
- Logger built-in with colored text, and a console GUI which uses the logging hook system
- Abstract system for accessing objects such as meshes and shaders through string names
- Virtual filesystem class translates paths to their real position, and handles file overrides much like the resource pack system in Minecraft

Again, the goal of the engine is to have as much customization as possible, while still maintaining ease of use.

## Bundled Dependencies
- AngelScript 2.34.0
- GLAD (OpenGL 3.3 Core, no extensions)
- GLFW
- GLM
- ImGUI
- nlohmann_json
- OpenAL Soft
- libogg
- libvorbis
- stb_image

## Compilation
- **CLion (recommended)**: The project will compile without any prior configuration. If developing on Windows, you will need to install the Windows SDK (see below).

- Visual Studio 2019: You will need to install the following components:

![image](https://user-images.githubusercontent.com/26600014/128105644-cfa92f30-dc96-4476-a4c9-8d8b5f3ce129.png)

Additionally, compiling with MSVC will produce an error for x64 targets, due to a bug with AngelScript. I strongly recommend compiling with MinGW if you can help it, or targeting x86 if you can't.

## Usage
Make sure to copy the `src/resources` folder to the executable directory in your buildscript, or it will fail due to missing assets.

A good example buildscript and application can be found at:

[https://github.com/craftablescience/ChiraEditor](https://github.com/craftablescience/ChiraEditor)
