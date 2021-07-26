# Chira Engine
A fully customizable (and heavily work-in-progress) game engine written in C++17.

## Features
There may be more that I have missed, but:
- As extensible as possible: register custom logging hooks, scripting providers, sound providers, etc.
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
I have only tested compiling this project in CLion. This is the recommended IDE for developing the application,
and if you import this project into CLion it will immediately be ready to run, with no further configuration necessary.

## Usage
Make sure to copy the `src/resources` folder to the executable directory in your buildscript, or it will fail due to missing assets.

Look at the files in the `example` directory for more information.
