cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
include(FetchContent)

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
set(CMAKE_CXX_STANDARD 20)

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

# Helper for enum options (pick between certain strings)
macro(option_enum)
    cmake_parse_arguments(IN "" "NAME;DEFAULT;DESCRIPTION" "OPTIONS" ${ARGN})
    set(${IN_NAME}_OPTIONS ${IN_OPTIONS})
    set(${IN_NAME} ${IN_DEFAULT} CACHE STRING ${IN_DESCRIPTION})
    set_property(CACHE ${IN_NAME} PROPERTY STRINGS ${IN_NAME}_OPTIONS)
    if(NOT ${IN_NAME} IN_LIST ${IN_NAME}_OPTIONS)
        message(FATAL_ERROR "${IN_NAME} (currently ${${IN_NAME}}) must be one of the following values: ${${IN_NAME}_OPTIONS}")
    endif()
endmacro()

# Options
include(CMakeDependentOption)
option(CHIRA_BUILD_EXTERNAL_TOOLS "Build various external tool executables" ON)
option(CHIRA_BUILD_INSTALLER "Build an installer for the compiled binaries" OFF)
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" ON)

option(CHIRA_BUILD_WITH_ASSERTS "Build Chira Engine with assertions enabled" ON)
cmake_dependent_option(CHIRA_BUILD_WITH_LTO "Build Chira Engine with Link-Time Optimizations" ON "NOT CHIRA_DEBUG_BUILD" OFF)
# Precompiled headers seem to make compilation slower when not using MSVC
cmake_dependent_option(CHIRA_BUILD_WITH_PCH "Build Chira Engine with precompiled headers" ON "CHIRA_COMPILER_MSVC" OFF)
# Only works with MSVC (needs to be dynamically linked for Windows Store applications)
cmake_dependent_option(CHIRA_BUILD_WITH_STATIC_MSVC_RUNTIME_LIBRARY "Build with the static MSVC runtime library" ON "CHIRA_COMPILER_MSVC;NOT WINDOWS_STORE" OFF)
option(CHIRA_BUILD_WITH_WARNINGS "Build Chira Engine with warnings enabled" ON)
option(CHIRA_ENABLE_CRT_SECURE_NO_WARNINGS "Disable the _CRT_SECURE_NO_WARNINGS warning" ON)
option(CHIRA_TREAT_WARNINGS_AS_ERRORS "Build Chira Engine with warnings treated as errors" OFF)

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

option(CHIRA_USE_DISCORD "Build Chira Engine with Discord rich presence features if possible" ON)
option(CHIRA_USE_STEAMWORKS "Build Chira Engine with Steamworks API features if possible" ON)

# Helper macro for printing a variable
macro(print_variable VARIABLE)
    message(STATUS "  ${VARIABLE}: ${${VARIABLE}}")
endmacro()

# Print out what is enabled and what isn't
message(STATUS "Options:")
print_variable(CHIRA_BUILD_EXTERNAL_TOOLS)
print_variable(CHIRA_BUILD_INSTALLER)
print_variable(CHIRA_BUILD_TESTS)
print_variable(CHIRA_BUILD_WITH_ASSERTS)
print_variable(CHIRA_BUILD_WITH_LTO)
print_variable(CHIRA_BUILD_WITH_PCH)
print_variable(CHIRA_BUILD_WITH_STATIC_MSVC_RUNTIME_LIBRARY)
print_variable(CHIRA_BUILD_WITH_WARNINGS)
print_variable(CHIRA_ENABLE_CRT_SECURE_NO_WARNINGS)
print_variable(CHIRA_TREAT_WARNINGS_AS_ERRORS)
print_variable(CHIRA_RENDER_BACKEND)
print_variable(CHIRA_RENDER_DEVICE)
print_variable(CHIRA_USE_DISCORD)
print_variable(CHIRA_USE_STEAMWORKS)

# Warn if libraries are not static by default
if(BUILD_SHARED_LIBS)
    message(WARNING "Building some libraries shared instead of static may cause weird errors. This is untested!")
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

    if(CHIRA_ENABLE_CRT_SECURE_NO_WARNINGS)
        target_compile_definitions(${TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    endif()

    if(CHIRA_TREAT_WARNINGS_AS_ERRORS)
        if(UNIX OR MINGW)
            target_compile_options(${TARGET} PRIVATE -Werror)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${TARGET} PRIVATE /WX)
        endif()
    endif()
endfunction()

# ENGINE
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/engine.cmake)

# CHIRATEST
if(CHIRA_BUILD_TESTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cmake)
endif()

# External tools
if(CHIRA_BUILD_EXTERNAL_TOOLS)
    set(CHIRA_EXTERNAL_TOOLS_NAME "external_tools")
    add_custom_target(${CHIRA_EXTERNAL_TOOLS_NAME})

    # Helper function to handle tools
    function(add_tool_executable NAME)
        cmake_parse_arguments(PARSE_ARGV 1 IN "GUI" "" "SOURCES;DEPS;INCLUDE_PATHS")
        if(WIN32)
            list(APPEND IN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/engine/platform/windows/app.rc)
        endif()
        if(IN_GUI AND NOT CHIRA_DEBUG_BUILD)
            add_executable(${NAME} WIN32 ${IN_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/tools/ToolHelpers.h)
            if(WIN32 AND MSVC)
                target_link_options(${NAME} PRIVATE "/ENTRY:mainCRTStartup")
            endif()
        else()
            add_executable(${NAME} ${IN_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/tools/ToolHelpers.h)
        endif()
        apply_optimizations(${NAME})
        target_link_libraries(${NAME} PRIVATE ${CHIRA_ENGINE_NAME} ${IN_DEPS})
        target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tools/${NAME} ${IN_INCLUDE_PATHS})
        add_dependencies(${CHIRA_EXTERNAL_TOOLS_NAME} ${NAME})
    endfunction()

    # CMDLTOOL
    include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmdltool/cmdltool.cmake)

    # EDITOR
    include(${CMAKE_CURRENT_SOURCE_DIR}/tools/editor/editor.cmake)
endif()

# Build installer
if(CHIRA_BUILD_INSTALLER)
    message(FATAL_ERROR "Installer is not implemented yet!")
    set(CPACK_PACKAGE_VENDOR "Chira Engine Developers" CACHE STRING "" FORCE)
    set(CPACK_GENERATOR "TGZ" CACHE STRING "" FORCE)
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An open source game engine for Windows, Linux, and macOS." CACHE STRING "" FORCE)
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding/steam/avatars/spider.png" CACHE STRING "" FORCE)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt" CACHE STRING "" FORCE)
    set(CPACK_MONOLITHIC_INSTALL TRUE CACHE BOOL "" FORCE)
    if(WIN32)
        #todo: NSIS or WIX
        #set(CPACK_GENERATOR "WIX" CACHE STRING "" FORCE)
        message(FATAL_ERROR "Sorry, no Windows installer is supported yet.")
    elseif(APPLE)
        set(CPACK_GENERATOR "Bundle" CACHE STRING "" FORCE)
        set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/branding/github/readme_banner.png" CACHE STRING "" FORCE)
        set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE TRUE CACHE BOOL "" FORCE)
        set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/Info.plist CACHE STRING "" FORCE)
        set(CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/AppIcon.icns CACHE STRING "" FORCE)
    elseif(UNIX)
        #todo: DEB and/or RPM
        message(FATAL_ERROR "Sorry, no Linux installer is supported yet.")
    endif()
    include(CPack)
endif()

# Copy resources
add_custom_target(CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)
add_dependencies(${CHIRA_ENGINE_NAME} CopyResources)
