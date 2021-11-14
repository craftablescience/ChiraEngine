# Put all libraries in the executable folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(CHIRA_BUILD_STATIC "Build Chira Engine's dependencies statically" OFF)
if(CHIRA_BUILD_STATIC OR MSVC)
    # Note: Distributing with static libs disabled means your distribution
    #       falls under LGPLv2.1 licensing, which you probably want to avoid.
    #       In other words, it's not recommended, but if you have to, it works.
    set(BUILD_SHARED_LIBS OFF)
else()
    # Build with dynamic (shared) dependencies
    set(BUILD_SHARED_LIBS ON)
endif()

# ANGELSCRIPT
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript)
list(APPEND ENGINE_LINK_LIBRARIES AngelScript)

# CURL
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(HTTP_ONLY ON CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
if(MSVC)
    set(CURL_STATIC_CRT ON CACHE BOOL "" FORCE)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl)
set(CURL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl/include)
set(CURL_LIBRARY libcurl)
list(APPEND ENGINE_LINK_LIBRARIES libcurl)

# CURLPP

if(MSVC)
    set(CURLPP_STATIC_CRT ON CACHE BOOL "" FORCE)
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

# BULLET
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
if(MSVC)
    set(BUILD_SHARED_LIBS OFF)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet)
list(APPEND ENGINE_LINK_LIBRARIES BulletDynamics BulletCollision LinearMath)
if(MSVC)
   set(BUILD_SHARED_LIBS ON)
endif()

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
include(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/CMakeLists.txt)

# STDUUID
set(UUID_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid)
list(APPEND ENGINE_LINK_LIBRARIES stduuid)

# TINYFILEDIALOGS
include(${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs/CMakeLists.txt)

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
    # TinyFileDialogs wants these
    list(APPEND ENGINE_LINK_LIBRARIES comdlg32 ole32)
elseif(UNIX)
    # ImGUI needs it on Linux
    list(APPEND ENGINE_LINK_LIBRARIES dl)
endif()

# IMGUI_MARKDOWN
list(APPEND ENGINE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown/imgui_markdown.h)

# CHIRAENGINE
include(${CMAKE_CURRENT_SOURCE_DIR}/config/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/entity/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/event/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/hook/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/i18n/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/input/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/loader/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/physics/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/render/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/resource/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/script/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/sound/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/utility/CMakeLists.txt)
add_library(ChiraEngine STATIC ${ENGINE_SOURCES})

target_compile_definitions(ChiraEngine PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)

target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript/addons)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curlpp/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/discord/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt/include)
target_include_directories(ChiraEngine SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include)
target_include_directories(ChiraEngine PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/src)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ogg/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/vorbis/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid/gsl)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stduuid/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown)
target_link_libraries(ChiraEngine PUBLIC ${OPENGL_LIBRARIES} ${ENGINE_LINK_LIBRARIES})

# Copy engine resources
add_custom_command(TARGET ChiraEngine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../resources/engine ${CMAKE_BINARY_DIR}/resources/engine)
