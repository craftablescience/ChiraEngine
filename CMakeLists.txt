cmake_minimum_required(VERSION 3.16)
include(CMakeDependentOption)


# Must set some Apple options before creating the project
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum OSX deployment version")
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
option(CHIRA_BUILD_EDITOR "Build the editor GUI application" ON)
cmake_dependent_option(CHIRA_BUILD_EDITOR_INSTALLER "Build an installer for the editor binaries" OFF "CHIRA_BUILD_EDITOR" OFF)
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" ON)

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
message(STATUS "  CHIRA_BUILD_WITH_LTO: ${CHIRA_BUILD_WITH_LTO}")
message(STATUS "  CHIRA_BUILD_WITH_PCH: ${CHIRA_BUILD_WITH_PCH}")
message(STATUS "  CHIRA_BUILD_WITH_WARNINGS: ${CHIRA_BUILD_WITH_WARNINGS}")
message(STATUS "  CHIRA_TREAT_WARNINGS_AS_ERRORS: ${CHIRA_TREAT_WARNINGS_AS_ERRORS}")
message(STATUS "  CHIRA_BUILD_HEADLESS: ${CHIRA_BUILD_HEADLESS}")
message(STATUS "  CHIRA_USE_DISCORD: ${CHIRA_USE_DISCORD}")
message(STATUS "  CHIRA_USE_STEAMWORKS: ${CHIRA_USE_STEAMWORKS}")


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


# Macro to apply optimization flags to multiple targets
macro(apply_optimizations target)
    if(CHIRA_DEBUG_BUILD)
        target_compile_definitions(${target} PRIVATE DEBUG)

        if(CHIRA_COMPILER_CLANG)
            target_compile_options(${target} PRIVATE -fno-limit-debug-info)
        endif()

        # On UNIX, build with debug friendly optimizations and debug symbols
        # On Windows, disable optimizations entirely
        if(UNIX OR MINGW)
            target_compile_options(${target} PRIVATE -Og -g)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${target} PRIVATE /Od)
        endif()

    else()
        target_compile_definitions(${target} PRIVATE RELEASE)

        # Build with optimizations and don't omit stack pointer for debugging
        if(UNIX OR MINGW)
            target_compile_options(${target} PRIVATE -O2 -fno-omit-frame-pointer)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${target} PRIVATE /O2)
        endif()
    endif()

    if(CHIRA_BUILD_WITH_WARNINGS)
        if(UNIX OR MINGW)
            target_compile_options(${target} PRIVATE -Wall -Wno-unused-parameter -Wno-sign-compare)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${target} PRIVATE /W4) # not all, because MSVC has loads of idiotic errors
        endif()
    endif()

    if(CHIRA_TREAT_WARNINGS_AS_ERRORS)
        if(UNIX OR MINGW)
            target_compile_options(${target} PRIVATE -Werror)
        elseif(CHIRA_COMPILER_MSVC)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    endif()
endmacro()


# Enable tests if wanted
if(CHIRA_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
endif()


# Add OS-specific libraries
if(APPLE)
    find_library(CORE_LIB CoreFoundation)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES ${CORE_LIB})
endif()


# Basic ImGui sources
list(APPEND IMGUI_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imstb_rectpack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imstb_textedit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imstb_truetype.h
        # Plus ImGui-Filebrowser for convenience
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui-filebrowser/imfilebrowser.h)
list(APPEND IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/imgui_widgets.cpp)


# Figure out what render backend and device backend we are using
if(CHIRA_BUILD_HEADLESS)
    #todo(render): implement a headless mode for testing
    #list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_BUILD_HEADLESS)
    message(FATAL_ERROR "Headless mode is not implemented yet!")
else()
    # The graphics API we use varies based on platform
    # macOS: OpenGL 4.1
    # Windows and Linux: OpenGL 4.3
    if(APPLE)
        set(GLAD_USE_GL_41 ON CACHE BOOL "" FORCE)
        list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_GL_41)
    else()
        set(GLAD_USE_GL_41 OFF CACHE BOOL "" FORCE)
        list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_GL_43)
    endif()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glad)
    list(APPEND CHIRA_ENGINE_DEFINITIONS IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glad/include)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glad)

    # SDL2
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/sdl2)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/sdl2/include)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES SDL2::SDL2)

    # TINYFILEDIALOGS
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/tinyfiledialogs)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/tinyfiledialogs)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES tinyfiledialogs)

    # Add ImGui platform
    list(APPEND IMGUI_HEADERS
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_sdl.h
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_opengl3.h)
    list(APPEND IMGUI_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_sdl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_opengl3.cpp)
    list(APPEND IMGUI_INCLUDE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/sdl2/include)
    list(APPEND IMGUI_LINK_LIBRARIES SDL2::SDL2)
endif()


# IMGUI
list(APPEND IMGUI_SOURCES ${IMGUI_HEADERS})
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui
        ${IMGUI_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE ${IMGUI_LINK_LIBRARIES})

list(APPEND CHIRA_ENGINE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui-filebrowser)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES imgui)


# ANGELSCRIPT
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/angelscript)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/angelscript/addons
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/angelscript/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES angelscript)


# DISCORD
if(CHIRA_USE_DISCORD)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/discord)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_DISCORD)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/discord/include)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES discord-rpc)
endif()


# FMT
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/fmt)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/fmt/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES fmt::fmt)


# GLM
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glm)
target_compile_definitions(glm INTERFACE GLM_FORCE_SILENT_WARNINGS)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glm)


# LIBLOADER
set(LIBLOADER_BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/libloader)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/libloader/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES LibLoader)


# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/json)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES nlohmann_json::nlohmann_json)


# STB
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stb)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stb)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stb)


# STEAMWORKS
if(CHIRA_USE_STEAMWORKS)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_STEAMWORKS)
endif()


# STDUUID
set(UUID_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stduuid)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stduuid/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stduuid)


# CHIRAENGINE
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/CMakeLists.txt)
list(APPEND CHIRA_ENGINE_SOURCES ${CHIRA_ENGINE_HEADERS})

add_library(${PROJECT_NAME} STATIC ${CHIRA_ENGINE_SOURCES})
apply_optimizations(${PROJECT_NAME})

if(CHIRA_BUILD_WITH_PCH)
    target_precompile_headers(${PROJECT_NAME} PRIVATE ${CHIRA_ENGINE_HEADERS})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC
        ${CHIRA_ENGINE_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CHIRA_ENGINE_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/engine)
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${CHIRA_ENGINE_LINK_LIBRARIES})


# CHIRAEDITOR
if(CHIRA_BUILD_EDITOR)
    include(${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt)
    if(WIN32)
        list(APPEND CHIRA_EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/editor/platform/windows/app.rc)
    endif()
    if(APPLE)
        list(APPEND CHIRA_APPLE_BUNDLE_FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/AppIcon.icns
                ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/Info.plist)
        list(APPEND CHIRA_EDITOR_SOURCES ${CHIRA_APPLE_BUNDLE_FILES})
    endif()
    add_executable(ChiraEditor ${CHIRA_EDITOR_SOURCES})
    apply_optimizations(ChiraEditor)
    if(APPLE)
        set_target_properties(ChiraEditor PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_FRAMEWORK_IDENTIFIER info.craftablescience.ChiraEditor
                BUNDLE "${CHIRA_APPLE_BUNDLE_FILES}")
    endif()
    target_link_libraries(ChiraEditor PRIVATE ${PROJECT_NAME})
    target_include_directories(ChiraEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/editor)

    if(CHIRA_BUILD_EDITOR_INSTALLER)
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
endif()


# CHIRATEST
if(CHIRA_BUILD_TESTS)
    add_executable(ChiraTest ${CHIRA_TEST_SOURCES})
    target_link_libraries(ChiraTest PUBLIC ${PROJECT_NAME} gtest_main)
    target_include_directories(ChiraTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    include(GoogleTest)
    gtest_discover_tests(ChiraTest)

    # Copy gtest binaries next to the executable
    add_custom_command(
            TARGET ChiraTest
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/bin ${CMAKE_BINARY_DIR})
endif()


# Copy resources
add_custom_target(${PROJECT_NAME}CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}CopyResources)
