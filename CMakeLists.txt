cmake_minimum_required(VERSION 3.16)
find_package(PkgConfig)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum OSX deployment version")

project(ChiraEngine HOMEPAGE_URL "https://github.com/craftablescience/ChiraEngine")
set(CMAKE_CXX_STANDARD 20)

# You should add this to your own CMake to load Chira Engine's dependencies
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)

# Put all libraries in the executable folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Compile with PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

# Macro to apply optimization flags to multiple targets
option(CHIRA_BUILD_WITH_WARNINGS "Build Chira Engine with warnings enabled" ON)
option(CHIRA_TREAT_WARNINGS_AS_ERRORS "Build Chira Engine with warnings treated as errors" OFF) # Currently catches warnings from libraries
macro(apply_optimizations target)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
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

    elseif(CMAKE_BUILD_TYPE MATCHES Release)
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
    
    # psst! didn't know where else to put this so here. We need this for xcode
    if(CMAKE_GENERATOR STREQUAL Xcode)
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CC "/usr/local/opt/llvm/bin/clang")
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CXX "/usr/local/opt/llvm/bin/clang++")
    endif()
endmacro()

# GOOGLETEST
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" ON)
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

# COREFOUNDATION
if(APPLE)
    find_library(CORE_LIB CoreFoundation)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES ${CORE_LIB})
endif()

# DISCORD
option(CHIRA_USE_DISCORD "Build Chira Engine with Discord rich presence features" ON)
if(CHIRA_USE_DISCORD)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/discord)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES discord-rpc)
endif()

# FMT
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/fmt)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES fmt::fmt)

# OPENGL
find_package(OpenGL REQUIRED)

# GLAD
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glad)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glad)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glfw)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glfw)

# GLM
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glm)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glm)

# IMGUI
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES imgui)

# LIBLOADER
set(LIBLOADER_BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/libloader)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES LibLoader)

# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/json)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES nlohmann_json::nlohmann_json)

# STB
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stb)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stb)

# STDUUID
set(UUID_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stduuid)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stduuid)

# TINYFILEDIALOGS
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/tinyfiledialogs)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES tinyfiledialogs)

# CHIRAENGINE
option(CHIRA_USE_STEAMWORKS "Build Chira Engine with Steamworks API features" ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/engine/CMakeLists.txt)
list(APPEND CHIRA_ENGINE_SOURCES ${CHIRA_ENGINE_HEADERS})

add_library(${PROJECT_NAME} STATIC ${CHIRA_ENGINE_SOURCES})
apply_optimizations(${PROJECT_NAME})

# Precompiled headers seem to make compilation slower when not using MSVC
if(MSVC)
    option(CHIRA_USE_PCH "Use precompiled headers when building Chira Engine" ON)
else()
    option(CHIRA_USE_PCH "Use precompiled headers when building Chira Engine" OFF)
endif()
if(CHIRA_USE_PCH)
    target_precompile_headers(${PROJECT_NAME} PRIVATE ${CHIRA_ENGINE_HEADERS})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)

if(CHIRA_USE_DISCORD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CHIRA_USE_DISCORD)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/discord/include)
endif()
if(CHIRA_USE_STEAMWORKS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CHIRA_USE_STEAMWORKS)
endif()

# Scripting languages
set(CHIRA_SCRIPT_LANG "AngelScript" CACHE STRING "Choose which scripting language this build of ChiraEngine will use" )
if (${CHIRA_SCRIPT_LANG} MATCHES "AngelScript")
    # ANGELSCRIPT
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/angelscript)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES angelscript)
    list(APPEND SCRIPT_LANG_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/angelscript/addons
                                    ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/angelscript/include)
elseif (${CHIRA_SCRIPT_LANG} MATCHES "CSharp")
    # MONO
    pkg_check_modules(MONO REQUIRED mono-2)
    target_compile_options(${PROJECT_NAME} PRIVATE ${MONO_CFLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${MONO_LDFLAGS})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        ${SCRIPT_LANG_INCLUDE}
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/fmt/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/libloader/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stduuid/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/tinyfiledialogs
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui-filebrowser)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} ${CHIRA_ENGINE_LINK_LIBRARIES})

# CHIRAEDITOR
option(CHIRA_BUILD_EDITOR "Build the editor GUI application" ON)
if(CHIRA_BUILD_EDITOR)
    include(${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt)
    if(WIN32)
        list(APPEND CHIRA_EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/platform/windows/icon.rc)
    endif()
    if(APPLE)
        list(APPEND CHIRA_APPLE_BUNDLE_FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/AppIcon.icns
                ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/Info.plist)
        list(APPEND CHIRA_EDITOR_SOURCES ${CHIRA_APPLE_BUNDLE_FILES})
    endif()
    add_executable(ChiraEditor ${CHIRA_EDITOR_SOURCES})
    apply_optimizations(ChiraEditor)
    if(WIN32)
        add_custom_target(
                CopyEditorIcon
                ALL
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/windows/icon.ico ${CMAKE_BINARY_DIR}/resources/platform/windows/icon.ico)
        add_dependencies(ChiraEditor CopyEditorIcon)
    endif()
    if(APPLE)
        set_target_properties(ChiraEditor PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_FRAMEWORK_IDENTIFIER info.craftablescience.ChiraEditor
                BUNDLE "${CHIRA_APPLE_BUNDLE_FILES}")
    endif()
    target_link_libraries(ChiraEditor PRIVATE ${PROJECT_NAME})
    target_include_directories(ChiraEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/editor)

    option(CHIRA_EDITOR_BUILD_INSTALLER "Build an installer for the editor binaries" OFF)
    if(CHIRA_EDITOR_BUILD_INSTALLER)
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
            set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/Info.plist CACHE STRING "" FORCE)
            set(CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/AppIcon.icns CACHE STRING "" FORCE)
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
add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)
