cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CMakeDependentOption)
include(FetchContent)
include(IncludeSubDirs)
include(OptionEnum)
include(PrintVariable)

# Must set some Apple options before creating the project
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum OSX deployment version")
    if(CMAKE_GENERATOR STREQUAL Xcode)
        set(CMAKE_XCODE_ATTRIBUTE_CC ${CMAKE_C_COMPILER})
        set(CMAKE_XCODE_ATTRIBUTE_CXX ${CMAKE_CXX_COMPILER})
    endif()
endif()

# Create the project
project(Chira HOMEPAGE_URL "https://github.com/craftablescience/ChiraEngine")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If we're not building a debug build just assume it's release
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CHIRA_DEBUG_BUILD ON)
    message(STATUS "Building debug build.")
else()
    set(CHIRA_DEBUG_BUILD OFF)
    message(STATUS "Building release build.")
endif()

# This is for detecting Clang, MSVC, and Clang-CL
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" CHIRA_COMPILER_ID)
string(TOLOWER "${CMAKE_CXX_SIMULATE_ID}" CHIRA_SIMULATE_ID)
set(CHIRA_COMPILER_CLANG OFF)
set(CHIRA_COMPILER_MSVC OFF)
if(MSVC)
    set(CHIRA_COMPILER_MSVC ON)
endif()
if(CHIRA_COMPILER_ID MATCHES "clang")
    if(NOT (CHIRA_SIMULATE_ID MATCHES "msvc"))
        set(CHIRA_COMPILER_CLANG ON)
    else()
        set(CHIRA_COMPILER_MSVC ON)
    endif()
endif()

# Options
option(CHIRA_DEVELOPER_BUILD "Build Chira Engine assuming the user is a developer building it from their IDE or terminal. DISABLE THIS IN PRODUCTION ENVIRONMENTS!" ON)

option(CHIRA_BUILD_EXTERNAL_TOOLS "Build various external tool executables." ON)
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests." ON)

option(CHIRA_BUILD_WITH_ASSERTS "Build Chira Engine with assertions enabled." ON)
cmake_dependent_option(CHIRA_BUILD_WITH_LTO "Build Chira Engine with Link-Time Optimizations." ON "NOT CHIRA_DEBUG_BUILD" OFF)
# Precompiled headers seem to make compilation slower when not using MSVC
cmake_dependent_option(CHIRA_BUILD_WITH_PCH "Build Chira Engine with precompiled headers." ON "CHIRA_COMPILER_MSVC" OFF)
# Only works with MSVC (needs to be dynamically linked for Windows Store applications)
cmake_dependent_option(CHIRA_BUILD_WITH_STATIC_MSVC_RUNTIME_LIBRARY "Build with the static MSVC runtime library" ON "CHIRA_COMPILER_MSVC;NOT WINDOWS_STORE" OFF)
option(CHIRA_BUILD_WITH_WARNINGS "Build Chira Engine with warnings enabled." ON)

option_enum(
        NAME "CHIRA_RENDER_BACKEND"
        DESCRIPTION "Override the default render backend. If set to AUTO, will choose the best backend for the current platform."
        DEFAULT "AUTO"
        OPTIONS "AUTO" "GL40" "GL41" "GL43" "SDLRENDERER")
option_enum(
        NAME "CHIRA_RENDER_DEVICE"
        DESCRIPTION "Override the default device backend."
        DEFAULT "SDL2"
        OPTIONS "SDL2" "HEADLESS")

option(CHIRA_USE_DISCORD "Build Chira Engine with Discord rich presence features if possible." ON)
option(CHIRA_USE_STEAMWORKS "Build Chira Engine with Steamworks API features if possible." ON)

# Print out what is enabled and what isn't
message(STATUS "Options:")
print_variables(
        CHIRA_DEVELOPER_BUILD
        CHIRA_BUILD_EXTERNAL_TOOLS
        CHIRA_BUILD_TESTS
        CHIRA_BUILD_WITH_ASSERTS
        CHIRA_BUILD_WITH_LTO
        CHIRA_BUILD_WITH_PCH
        CHIRA_BUILD_WITH_STATIC_MSVC_RUNTIME_LIBRARY
        CHIRA_BUILD_WITH_WARNINGS
        CHIRA_RENDER_BACKEND
        CHIRA_RENDER_DEVICE
        CHIRA_USE_DISCORD
        CHIRA_USE_STEAMWORKS)

# Set developer build
if(CHIRA_DEVELOPER_BUILD)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_DEVELOPER_BUILD)
endif()

# Set libraries to static by default
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Building some libraries shared instead of static may cause weird errors.")

# Set runpath
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")

# Put all dynamic libraries in the executable directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Compile with PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Compile with LTO if supported
set(CHIRA_BUILD_WITH_LTO_INTERNAL OFF)
if(CHIRA_BUILD_WITH_LTO)
    set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT CHIRA_BUILD_WITH_LTO_INTERNAL)
endif()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${CHIRA_BUILD_WITH_LTO_INTERNAL})

# Enable/disable assertions
if(CHIRA_BUILD_WITH_ASSERTS)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_BUILD_WITH_ASSERTS)
endif()

# Set MSVC static/dynamic CRT
if(CHIRA_BUILD_WITH_STATIC_MSVC_RUNTIME_LIBRARY)
    if(CHIRA_DEBUG_BUILD)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
else()
    if(CHIRA_DEBUG_BUILD)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()
endif()
if(CHIRA_COMPILER_MSVC)
    message(STATUS "Building with ${CMAKE_MSVC_RUNTIME_LIBRARY} CRT.")
endif()

# Function to apply optimization flags to multiple targets
function(apply_optimizations TARGET)
    # Set optimization flags
    if(CHIRA_DEBUG_BUILD)
        target_compile_definitions(${TARGET} PRIVATE DEBUG)

        if(CHIRA_COMPILER_CLANG)
            target_compile_options(${TARGET} PRIVATE -fno-limit-debug-info)
        endif()

        # On UNIX, build with debug friendly optimizations and debug symbols
        # On Windows, disable optimizations entirely
        if(UNIX OR MINGW)
            target_compile_options(${TARGET} PRIVATE -Og -g)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${TARGET} PRIVATE /Od)
        endif()
    else()
        target_compile_definitions(${TARGET} PRIVATE RELEASE)

        # Build with optimizations and don't omit stack pointer for debugging
        if(UNIX OR MINGW)
            target_compile_options(${TARGET} PRIVATE -O2 -fno-omit-frame-pointer)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${TARGET} PRIVATE /O2)
        endif()
    endif()

    # Enable warnings
    if(CHIRA_BUILD_WITH_WARNINGS)
        if(UNIX OR MINGW)
            target_compile_options(${TARGET} PRIVATE -Wall -Wno-unused-parameter -Wno-sign-compare)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${TARGET} PRIVATE /W4) # not all, because MSVC has loads of idiotic errors
        endif()
    endif()
endfunction()

# Add engine
include_subdirs(engine)

# Add tools
if(CHIRA_BUILD_EXTERNAL_TOOLS)
    include_subdirs(tools)
endif()

# Warn user about the developer build
if(CHIRA_DEVELOPER_BUILD)
    message(NOTICE "CHIRA_DEVELOPER_BUILD is currently ENABLED. Remember to disable this variable when compiling production binaries!")
endif()
