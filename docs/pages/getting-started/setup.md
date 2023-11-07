# Creating a Chira Engine Project {#createchiraengproj}
This guide will teach you how to setup a project using Chira Engine.

[TOC]

# Prerequisites:
You'll need a proper development environment setup in order to build a game using Chira Engine. This usually consists of an IDE, git, and a compiler.

CLion is recommended as the IDE for Chira Engine Projects as it compiles out of the box with little/no configuration as long as you have the dependencies installed.

The recommended compilers are as follows:

**Windows:** MSVC

**Linux:** GCC

**macOS:** Clang*

> *NOTE: The version of Clang Apple provides with XCode has been found to be problematic. If Chira Engine doesn't compile using XCode's Clang see [Using Homebrew LLVM (macOS)]()

## macOS Dependencies:
XCode provides most if not all missing dependencies and even the compiler

## Linux Dependencies
On Debian systems run the following:
```
sudo apt update && sudo apt install cmake build-essential xorg-dev mesa-common-dev mesa-utils
```

## Windows Dependencies
On Windows you will need to install (most of) the following components of Visual Studio regardless of your IDE choice. Some are not required, like the AddressSanitizer and Just-In-Time debugger. This is an old screenshot, make any upgrades to the versions of these components that you see fit.

![image](https://user-images.githubusercontent.com/26600014/128105644-cfa92f30-dc96-4476-a4c9-8d8b5f3ce129.png)

# Creating the base project
Now that we've taken care of the basics we can now get started on actually setting up our ChiraEngine project.

Start out by creating a new folder. For this guide we'll be using `MyProject`

Inside of this new folder create a new git repository and add ChiraEngine as a submodule using the following commands.
 
```
git init

git submodule add https://github.com/craftablescience/ChiraEngine.git
```

Next we'll open our IDE and create a new file in the root of our project called `CMakeLists.txt`

Inside of this file place
```CMake
cmake_minimum_required(VERSION 3.16)

project(MyProject)

# load Chira Engine's dependencies
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)

option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" OFF)
# uncomment the line below to disable compiling the editor alongside your project
# option(CHIRA_BUILD_EDITOR "Build the editor GUI application" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ChiraEngine/CMakeLists.txt)

include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)
add_executable(${PROJECT_NAME} ${MY_PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ChiraEngine)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
```
TODO: Finish this section
