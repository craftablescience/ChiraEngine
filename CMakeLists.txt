cmake_minimum_required(VERSION 3.16)

# Must set some Apple options before creating the project
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OSX deployment version")
    if(CMAKE_GENERATOR STREQUAL Xcode)
        set(CMAKE_XCODE_ATTRIBUTE_CC ${CMAKE_C_COMPILER})
        set(CMAKE_XCODE_ATTRIBUTE_CXX ${CMAKE_CXX_COMPILER})
    endif()
endif()

# Create the project
project(ChiraEngine HOMEPAGE_URL "https://github.com/craftablescience/ChiraEngine")
set(CMAKE_CXX_STANDARD 20)

# If we're not building a debug build just assume it's release
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CHIRA_DEBUG_BUILD ON)
    message(STATUS "Building debug build.")
else()
    set(CHIRA_DEBUG_BUILD OFF)
    message(STATUS "Building release build.")
endif()

# Options
include(CMakeDependentOption)
option(CHIRA_BUILD_EDITOR "Build the editor GUI application" ON)
option(CHIRA_BUILD_TOOLS "Build various external tool executables" ON)
cmake_dependent_option(CHIRA_BUILD_EDITOR_INSTALLER "Build an installer for the editor binaries" OFF "CHIRA_BUILD_EDITOR" OFF)
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" ON)

option(CHIRA_BUILD_WITH_ASSERTS "Build Chira Engine with assertions enabled" ON)
cmake_dependent_option(CHIRA_BUILD_WITH_LTO "Build Chira Engine with Link-Time Optimizations" ON "NOT CHIRA_DEBUG_BUILD" OFF)
# Precompiled headers seem to make compilation slower when not using MSVC
cmake_dependent_option(CHIRA_BUILD_WITH_PCH "Build Chira Engine with precompiled headers" ON "CHIRA_COMPILER_MSVC" OFF)
option(CHIRA_BUILD_WITH_WARNINGS "Build Chira Engine with warnings enabled" ON)
option(CHIRA_TREAT_WARNINGS_AS_ERRORS "Build Chira Engine with warnings treated as errors" OFF)

option(CHIRA_BUILD_HEADLESS "Build Chira Engine without any visible window(s). Zero reliance on SDL. Primarily for testing purposes." OFF)
option(CHIRA_USE_DISCORD "Build Chira Engine with Discord rich presence features if possible" ON)
option(CHIRA_USE_STEAMWORKS "Build Chira Engine with Steamworks API features if possible" ON)

# Print out what is enabled and what isn't
message(STATUS "Options:")
message(STATUS "  CHIRA_BUILD_EDITOR: ${CHIRA_BUILD_EDITOR}")
message(STATUS "  CHIRA_BUILD_EDITOR_INSTALLER: ${CHIRA_BUILD_EDITOR_INSTALLER}")
message(STATUS "  CHIRA_BUILD_TESTS: ${CHIRA_BUILD_TESTS}")
message(STATUS "  CHIRA_BUILD_WITH_ASSERTS: ${CHIRA_BUILD_WITH_ASSERTS}")
message(STATUS "  CHIRA_BUILD_WITH_LTO: ${CHIRA_BUILD_WITH_LTO}")
message(STATUS "  CHIRA_BUILD_WITH_PCH: ${CHIRA_BUILD_WITH_PCH}")
message(STATUS "  CHIRA_BUILD_WITH_WARNINGS: ${CHIRA_BUILD_WITH_WARNINGS}")
message(STATUS "  CHIRA_TREAT_WARNINGS_AS_ERRORS: ${CHIRA_TREAT_WARNINGS_AS_ERRORS}")
message(STATUS "  CHIRA_BUILD_HEADLESS: ${CHIRA_BUILD_HEADLESS}")
message(STATUS "  CHIRA_USE_DISCORD: ${CHIRA_USE_DISCORD}")
message(STATUS "  CHIRA_USE_STEAMWORKS: ${CHIRA_USE_STEAMWORKS}")

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

# You should add this to your own CMake to load Chira Engine's dependencies in non-Windows environments
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)

# Put all dynamic libraries in the executable directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Compile with PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Compile with LTO if supported
set(CHIRA_BUILD_WITH_LTO_INTERNAL OFF)
if(CHIRA_BUILD_WITH_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT CHIRA_BUILD_WITH_LTO_INTERNAL)
endif()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${CHIRA_BUILD_WITH_LTO_INTERNAL})

# Enable/disable assertions
if(CHIRA_BUILD_WITH_ASSERTS)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_BUILD_WITH_ASSERTS)
endif()

# Function to apply optimization flags to multiple targets
function(apply_optimizations TARGET)
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

    if(CHIRA_BUILD_WITH_WARNINGS)
        if(UNIX OR MINGW)
            target_compile_options(${TARGET} PRIVATE -Wall -Wno-unused-parameter -Wno-sign-compare)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${TARGET} PRIVATE /W4) # not all, because MSVC has loads of idiotic errors
        endif()
    endif()

    if(CHIRA_TREAT_WARNINGS_AS_ERRORS)
        if(UNIX OR MINGW)
            target_compile_options(${TARGET} PRIVATE -Werror)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${TARGET} PRIVATE /WX)
        endif()
    endif()
endfunction()

# CHIRAENGINE
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/engine.cmake)

if(CHIRA_BUILD_EDITOR)
    # CHIRAEDITOR
    include(${CMAKE_CURRENT_SOURCE_DIR}/editor/editor.cmake)
endif()

if(CHIRA_BUILD_TESTS)
    # CHIRATEST
    include(${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cmake)
endif()

if(CHIRA_BUILD_TOOLS)
    # Helper function to handle tools
    function(add_tool_executable NAME)
        cmake_parse_arguments(PARSE_ARGV 1 IN "" "" "SOURCES;DEPS;INCLUDE_PATHS")
        add_executable(${NAME} ${IN_SOURCES})
        apply_optimizations(${NAME})
        target_link_libraries(${NAME} PRIVATE ${CHIRA_ENGINE_NAME} ${IN_DEPS})
        target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tools/${NAME} ${IN_INCLUDE_PATHS})
    endfunction()

    # CMDLTOOL
    include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmdltool/cmdltool.cmake)
endif()

# Copy resources
add_custom_target(CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)
add_dependencies(${CHIRA_ENGINE_NAME} CopyResources)
