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

# Compile with PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# This is for detecting Clang, MSVC, and Clang-CL
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" CHIRA_COMPILER_ID)
string(TOLOWER "${CMAKE_CXX_SIMULATE_ID}" CHIRA_SIMULATE_ID)
set(CHIRA_COMPILER_CLANG OFF)
set(CHIRA_COMPILER_MSVC  OFF)
if(MSVC)
    set(CHIRA_COMPILER_MSVC ON)
endif()
if(CHIRA_COMPILER_ID MATCHES "clang")
    if(NOT (CHIRA_SIMULATE_ID MATCHES "msvc"))
        set(CHIRA_COMPILER_CLANG ON)
    else()
        set(CHIRA_COMPILER_MSVC  ON)
    endif()
endif()

option(CHIRA_BUILD_WITH_MSVC_COMPAT "Build most of Chira Engine's dependencies statically" OFF)
if(CHIRA_COMPILER_MSVC)
    set(CHIRA_BUILD_WITH_MSVC_COMPAT ON)
endif()
if(CHIRA_BUILD_WITH_MSVC_COMPAT)
    # Note: Distributing with statically build libraries means your distribution
    #       might fall under LGPLv2.1 licensing, which you probably want to avoid.
    #       In other words, it's not recommended, but if you have to, it works.
    set(BUILD_SHARED_LIBS OFF)
    add_definitions(-DLIBTYPE=STATIC)
else()
    set(BUILD_SHARED_LIBS ON)
endif()

# GOOGLETEST
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" ON)
if(CHIRA_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
    )
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/engine/CMakeLists.txt)
endif()

if(WIN32 AND ((NOT CHIRA_BUILD_WITH_MSVC_COMPAT) AND CHIRA_COMPILER_CLANG))
    # Program has a fit at runtime if AngelScript and Bullet aren't statically linked compiling with clang
    set(BUILD_SHARED_LIBS OFF)
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
set(BUILD_ENET              OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET          OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet)
list(APPEND ENGINE_LINK_LIBRARIES Bullet3Common BulletDynamics BulletCollision LinearMath)

if(WIN32 AND ((NOT CHIRA_BUILD_WITH_MSVC_COMPAT) AND CHIRA_COMPILER_CLANG))
    # And re-enable building as shared libraries
    set(BUILD_SHARED_LIBS ON)
endif()

# CURL
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(HTTP_ONLY ON CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
if(CHIRA_BUILD_WITH_MSVC_COMPAT AND CHIRA_COMPILER_MSVC)
    set(CURL_STATIC_CRT ON CACHE BOOL "" FORCE)
else()
    set(CURL_STATIC_CRT OFF CACHE BOOL "" FORCE)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl)
set(CURL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl/include)
set(CURL_LIBRARY libcurl)
list(APPEND ENGINE_LINK_LIBRARIES libcurl)

# CURLPP
if(CHIRA_BUILD_WITH_MSVC_COMPAT AND CHIRA_COMPILER_MSVC)
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

# IMGUI
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
list(APPEND ENGINE_LINK_LIBRARIES ImGui)

# IMGUI_MARKDOWN
list(APPEND ENGINE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown/imgui_markdown.h)

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

# LIBLOADER
set(LIBLOADER_BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/libloader)
list(APPEND ENGINE_LINK_LIBRARIES LibLoader)

# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/json)
list(APPEND ENGINE_LINK_LIBRARIES nlohmann_json::nlohmann_json)

# STB
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
list(APPEND ENGINE_LINK_LIBRARIES stb)

# STDUUID
set(UUID_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid)
list(APPEND ENGINE_LINK_LIBRARIES stduuid)

# TINYFILEDIALOGS
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs)
list(APPEND ENGINE_LINK_LIBRARIES tinyfiledialogs)

# CHIRAENGINE
option(CHIRA_BUILD_WITH_STEAMWORKS "Build Chira Engine with Steamworks API features" ON)
option(CHIRA_BUILD_WITH_MULTIWINDOW "Build Chira Engine with the ability to create multiple GLFW windows (currently broken)" OFF)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/CMakeLists.txt)
add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES})

if(CHIRA_BUILD_WITH_STEAMWORKS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CHIRA_BUILD_WITH_STEAMWORKS)
endif()
if(CHIRA_BUILD_WITH_MULTIWINDOW)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CHIRA_BUILD_WITH_MULTIWINDOW)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)
if(CHIRA_BUILD_WITH_MSVC_COMPAT)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CHIRA_BUILD_WITH_MSVC_COMPAT)
endif()
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
    if(CHIRA_COMPILER_CLANG)
        target_compile_options(${PROJECT_NAME} PUBLIC -fno-limit-debug-info)
    endif()
    if(UNIX OR MINGW)
        # On UNIX, build with debug friendly optimizations and debug symbols
        # On windows, just disable optimizations entirely
        target_compile_options(${PROJECT_NAME} PUBLIC -Og -g)
    elseif(CHIRA_COMPILER_MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /Od)
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RELEASE)
    # Build with max optimizations and don't omit stack ptr for debugging reasons
    if(UNIX OR MINGW)
        target_compile_options(${PROJECT_NAME} PUBLIC -O3 -fno-omit-frame-pointer)
    elseif(CHIRA_COMPILER_MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /O2)
    endif()
endif()

option(CHIRA_BUILD_WITH_WARNINGS "Build Chira Engine with warnings enabled" ON)
if(CHIRA_BUILD_WITH_WARNINGS)
    if(UNIX OR MINGW)
        target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
    elseif(CHIRA_COMPILER_MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /W4) # not all, because MSVC has loads of idiotic errors
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
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libloader/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ogg/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/vorbis/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid/gsl)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} ${ENGINE_LINK_LIBRARIES})

# CHIRAENGINEGUI
option(CHIRA_BUILD_EDITOR "Build the editor GUI application" ON)
if(CHIRA_BUILD_EDITOR)
    include(${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt)
    if(WIN32)
        list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/editor/win32/project.rc)
    endif()
    add_executable(ChiraEngineGui ${EDITOR_SOURCES})
    target_link_libraries(ChiraEngineGui PRIVATE ${PROJECT_NAME} ${OPENGL_LIBRARIES})
    target_include_directories(ChiraEngineGui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/editor)
endif()

# CHIRAMODELVIEWER
option(CHIRA_BUILD_MODEL_VIEWER "Build the Chira Model compiler" ON)
if(CHIRA_BUILD_MODEL_VIEWER)
    include(${CMAKE_CURRENT_SOURCE_DIR}/modelviewer/CMakeLists.txt)
    if(WIN32)
        list(APPEND MODEL_VIEWER_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/modelviewer/win32/project.rc)
    endif()
    add_executable(ChiraModelViewer ${MODEL_VIEWER_SOURCES})
    target_link_libraries(ChiraModelViewer PRIVATE ${PROJECT_NAME} ${OPENGL_LIBRARIES})
    target_include_directories(ChiraModelViewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modelviewer)
endif()

if(CHIRA_BUILD_TESTS)
    add_executable(ChiraTest ${TEST_SOURCES})
    target_link_libraries(ChiraTest PUBLIC ${PROJECT_NAME} ${OPENGL_LIBRARIES} gtest_main)
    include(GoogleTest)
    gtest_discover_tests(ChiraTest)
endif()

# Compiling for Windows requires the resources be copied before the RC file is compiled, for executables that have an icon
# However, I'd like to avoid making a custom target, perhaps someday the Linux method can be used for both platforms

# Copy engine resources
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/engine ${CMAKE_BINARY_DIR}/resources/engine)

# Copy editor resources
if(CHIRA_BUILD_EDITOR)
    if(WIN32)
        # Resource files
        add_custom_target(copyEditorResources ALL
                COMMAND ${CMAKE_COMMAND}
                -E copy_directory ${CMAKE_SOURCE_DIR}/resources/editor ${CMAKE_BINARY_DIR}/resources/editor)
        add_dependencies(ChiraEngineGui copyEditorResources)
    else()
        # Resource files
        add_custom_command(TARGET ChiraEngineGui PRE_BUILD
                COMMAND ${CMAKE_COMMAND}
                -E copy_directory ${CMAKE_SOURCE_DIR}/resources/editor ${CMAKE_BINARY_DIR}/resources/editor)
    endif()
endif()

# Copy model viewer resources
if(CHIRA_BUILD_MODEL_VIEWER)
    if(WIN32)
        # Resource files
        add_custom_target(copyChiraModelViewerResources ALL
                COMMAND ${CMAKE_COMMAND}
                -E copy_directory ${CMAKE_SOURCE_DIR}/resources/modelviewer ${CMAKE_BINARY_DIR}/resources/modelviewer)
        add_dependencies(ChiraModelViewer copyChiraModelViewerResources)
    else()
        # Resource files
        add_custom_command(TARGET ChiraModelViewer PRE_BUILD
                COMMAND ${CMAKE_COMMAND}
                -E copy_directory ${CMAKE_SOURCE_DIR}/resources/modelviewer ${CMAKE_BINARY_DIR}/resources/modelviewer)
    endif()
endif()

# Copy test resources
if(CHIRA_BUILD_TESTS)
    add_custom_command(TARGET ChiraTest PRE_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/test ${CMAKE_BINARY_DIR}/resources/test)
    add_custom_command(TARGET ChiraTest PRE_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/bin/ ${CMAKE_BINARY_DIR})
endif()
