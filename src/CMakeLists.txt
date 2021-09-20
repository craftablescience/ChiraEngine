# ANGELSCRIPT
include(${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript/CMakeLists.txt)

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
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet)
list(APPEND ENGINE_LINK_LIBRARIES BulletDynamics BulletCollision LinearMath)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.h)
if(WIN32 AND (NOT MSVC))
    # Windows IME support
    list(APPEND ENGINE_LINK_LIBRARIES imm32)
endif()

# IMGUI_MARKDOWN
list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown/imgui_markdown.h)

# CHIRAENGINE
include(${CMAKE_CURRENT_SOURCE_DIR}/config/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/i18n/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/implementation/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/input/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/loader/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/physics/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/render/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/resource/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/script/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/sound/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/ui/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/utility/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/wec/CMakeLists.txt)
add_library(ChiraEngine STATIC ${ENGINE_SOURCES})

target_compile_definitions(ChiraEngine PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_GLAD2)

target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/angelscript/addons)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/discord/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt/include)
target_include_directories(ChiraEngine SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include)
target_include_directories(ChiraEngine PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/src)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ogg/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/vorbis/include)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyfiledialogs)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
target_include_directories(ChiraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_markdown)
target_link_libraries(ChiraEngine ${OPENGL_LIBRARIES} ${ENGINE_LINK_LIBRARIES})
