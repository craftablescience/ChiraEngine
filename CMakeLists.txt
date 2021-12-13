cmake_minimum_required(VERSION 3.15)
project(ChiraEngine)
set(CMAKE_CXX_STANDARD 17)

# You should add this to your own CMake to load Chira Engine's dependencies
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)

# Put all libraries in the executable folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# This is mostly for detecting Clang
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" COMPILER_ID)

# Global compile options
if(UNIX OR MINGW)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

option(CHIRA_BUILD_STATIC "Build Chira Engine's dependencies statically" OFF)
# todo: clang kind of partially works with shared libs? look into this later
if(MSVC OR (COMPILER_ID MATCHES "clang"))
    set(CHIRA_BUILD_STATIC ON)
endif()
if(CHIRA_BUILD_STATIC)
    # Note: Distributing with static libs disabled means your distribution
    #       falls under LGPLv2.1 licensing, which you probably want to avoid.
    #       In other words, it's not recommended, but if you have to, it works.
    # todo: Currently it appears curlpp and OpenAL ignore this
    set(BUILD_SHARED_LIBS OFF)
    add_definitions(-DLIBTYPE=STATIC)
else()
    # Build with dynamic (shared) dependencies
    set(BUILD_SHARED_LIBS ON)
endif()

# GOOGLETEST
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" ON)
if(CHIRA_BUILD_TESTS AND (NOT MSVC))
    # For some reason Visual Studio doesn't like this
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/src/CMakeLists.txt)
endif()

# ANGELSCRIPT
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript)
list(APPEND ENGINE_LINK_LIBRARIES angelscript)

# BULLET
cmake_policy(SET CMP0077 NEW)
set(PKGCONFIG_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS         OFF CACHE BOOL "" FORCE)
set(USE_GLUT                OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS     OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS     OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS            OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS        OFF CACHE BOOL "" FORCE)
# No networking support... yet
set(BUILD_ENET              OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET          OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet)
list(APPEND ENGINE_LINK_LIBRARIES BulletDynamics BulletCollision LinearMath)

# CURL
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(HTTP_ONLY ON CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
if(CHIRA_BUILD_STATIC AND MSVC)
    set(CURL_STATIC_CRT ON CACHE BOOL "" FORCE)
else()
    set(CURL_STATIC_CRT OFF CACHE BOOL "" FORCE)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl)
set(CURL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl/include)
set(CURL_LIBRARY libcurl)
list(APPEND ENGINE_LINK_LIBRARIES libcurl)

# CURLPP
if(CHIRA_BUILD_STATIC AND MSVC)
    set(CURLPP_STATIC_CRT ON CACHE BOOL "" FORCE)
else()
    set(CURLPP_STATIC_CRT OFF CACHE BOOL "" FORCE)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/curlpp)
list(APPEND ENGINE_LINK_LIBRARIES curlpp)

# DISCORD
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/discord)
list(APPEND ENGINE_LINK_LIBRARIES discord-rpc)

# FMT
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt)
list(APPEND ENGINE_LINK_LIBRARIES fmt::fmt)

# OPENGL
find_package(OpenGL REQUIRED)

# GLAD
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad)
list(APPEND ENGINE_LINK_LIBRARIES glad)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw)
list(APPEND ENGINE_LINK_LIBRARIES glfw)

# GLM
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm)
list(APPEND ENGINE_LINK_LIBRARIES glm::glm)

# OPENAL
set(ALSOFT_EXAMPLES         OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_NO_CONFIG_UTIL   OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS            OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS    OFF CACHE BOOL "" FORCE)
set(ALSOFT_REQUIRE_OPENSL   OFF CACHE BOOL "" FORCE)
set(ALSOFT_REQUIRE_OBOE     OFF CACHE BOOL "" FORCE)
set(ALSOFT_REQUIRE_SDL2     OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/openalsoft)
list(APPEND ENGINE_LINK_LIBRARIES OpenAL)

# LIBOGG and LIBVORBIS
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# LIBOGG
set(INSTALL_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/ogg)
list(APPEND ENGINE_LINK_LIBRARIES ogg)

# LIBVORBIS
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/vorbis)
list(APPEND ENGINE_LINK_LIBRARIES vorbis vorbisenc vorbisfile)

# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/json)
list(APPEND ENGINE_LINK_LIBRARIES nlohmann_json::nlohmann_json)

# STB
if(CHIRA_BUILD_STATIC)
    list(APPEND ENGINE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/stb_image.h)
else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
    list(APPEND ENGINE_LINK_LIBRARIES stb)
endif()

# STDUUID
set(UUID_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid)
list(APPEND ENGINE_LINK_LIBRARIES stduuid)

# TINYFILEDIALOGS
if(CHIRA_BUILD_STATIC)
    list(APPEND ENGINE_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.h)
else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs)
    list(APPEND ENGINE_LINK_LIBRARIES tinyfiledialogs)
endif()

# IMGUI
list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imstb_rectpack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imstb_textedit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imstb_truetype.h
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_glfw.h
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.h
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/misc/cpp/imgui_stdlib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/misc/cpp/imgui_stdlib.h)
if(WIN32)
    # Windows IME support
    list(APPEND ENGINE_LINK_LIBRARIES imm32)
elseif(UNIX)
    # ImGUI needs it on Linux
    list(APPEND ENGINE_LINK_LIBRARIES dl)
endif()

# IMGUI_MARKDOWN
list(APPEND ENGINE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown/imgui_markdown.h)

# CHIRAENGINE
include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)
add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)
if(CHIRA_BUILD_STATIC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CHIRA_BUILD_STATIC)
endif()
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
    if(COMPILER_ID MATCHES "clang")
        target_compile_options(${PROJECT_NAME} PUBLIC -fno-limit-debug-info)
    endif()
    if(UNIX OR MINGW)
        # On UNIX, build with debug friendly optimizations and debug symbols
        # On windows, just disable optimizations entirely
        target_compile_options(${PROJECT_NAME} PUBLIC -Og -g)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /Od)
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DRELEASE)
    # Build with max optimizations and don't omit stack ptr for debugging reasons
    if(UNIX OR MINGW)
        target_compile_options(${PROJECT_NAME} PUBLIC -O3 -fno-omit-frame-pointer)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /O2)
    endif()
endif()

option(CHIRA_BUILD_WITH_WARNINGS "Build Chira Engine with warnings enabled" ON)
if(CHIRA_BUILD_WITH_WARNINGS)
    if(UNIX OR MINGW)
        target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /Wall)
    endif()
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript/addons)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curlpp/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/discord/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt/include)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ogg/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/vorbis/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid/gsl)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} ${ENGINE_LINK_LIBRARIES})

# Copy engine resources
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/engine ${CMAKE_BINARY_DIR}/resources/engine)

# CHIRAENGINEGUI
option(CHIRA_BUILD_EDITOR "Build the editor GUI application" ON)
if(CHIRA_BUILD_EDITOR)
    include(${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt)
    if(WIN32)
        list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/editor/win32/project.rc)
    endif()
    add_executable(ChiraEngineGui ${EDITOR_SOURCES})
    target_link_libraries(ChiraEngineGui ${PROJECT_NAME} ${OPENGL_LIBRARIES})
    target_include_directories(ChiraEngineGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/editor)
endif()

if(CHIRA_BUILD_TESTS AND (NOT MSVC))
    add_executable(ChiraTest ${TEST_SOURCES})
    target_include_directories(ChiraTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(ChiraTest PUBLIC ${PROJECT_NAME} ${OPENGL_LIBRARIES} gtest_main)
    include(GoogleTest)
    gtest_discover_tests(ChiraTest)
endif()

# Compiling for Windows requires the resources be copied before the RC file is compiled
# However, I'd like to avoid making a custom target, perhaps someday the Linux method can be used for both platforms
if(CHIRA_BUILD_EDITOR)
    if(WIN32)
        # Resource files
        add_custom_target(copyResources ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/editor ${CMAKE_BINARY_DIR}/resources/editor)
        add_dependencies(${PROJECT_NAME} copyResources)
    else()
        # Resource files
        add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/editor ${CMAKE_BINARY_DIR}/resources/editor)
    endif()
endif()

# Copy engine resources for tests (again...)
if(CHIRA_BUILD_TESTS AND (NOT MSVC))
    add_custom_command(TARGET ChiraTest PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/engine/ ${CMAKE_BINARY_DIR}/resources/engine/)
    add_custom_command(TARGET ChiraTest PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/bin/ ${CMAKE_BINARY_DIR})
endif()
