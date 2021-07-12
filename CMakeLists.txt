cmake_minimum_required(VERSION 3.20)
project(BasicGameEngine)
find_package(OpenGL REQUIRED)
set(CMAKE_CXX_STANDARD 20)

# Definitions
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()

# GLAD
include(third_party/glad/CMakeLists.txt)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# GLM
add_subdirectory(third_party/glm)

# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(third_party/json)

# STB
include(third_party/stb/CMakeLists.txt)

# IMGUI
list(APPEND ENGINE_SOURCES
        third_party/imgui/imconfig.h
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_demo.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_internal.h
        third_party/imgui/imgui_tables.cpp
        third_party/imgui/imgui_widgets.cpp
        third_party/imgui/imstb_rectpack.h
        third_party/imgui/imstb_textedit.h
        third_party/imgui/imstb_truetype.h
        third_party/imgui/backends/imgui_impl_glfw.cpp
        third_party/imgui/backends/imgui_impl_glfw.h
        third_party/imgui/backends/imgui_impl_opengl3.cpp
        third_party/imgui/backends/imgui_impl_opengl3.h)

# BASICGAMEENGINE
include(src/core/CMakeLists.txt)
include(src/input/CMakeLists.txt)
include(src/loader/CMakeLists.txt)
include(src/render/CMakeLists.txt)
include(src/utility/CMakeLists.txt)
add_executable(${PROJECT_NAME} example/demo.cpp ${ENGINE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE third_party/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE third_party/stb)
target_include_directories(${PROJECT_NAME} PRIVATE third_party/imgui)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw nlohmann_json::nlohmann_json glm::glm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

# Copy example resources
add_custom_target(copyEngineResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/basicgameengine/
        ${CMAKE_BINARY_DIR}/resources/basicgameengine/)
add_dependencies(${PROJECT_NAME} copyEngineResources)
add_custom_target(copyDemoResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/example/resources/
        ${CMAKE_BINARY_DIR}/resources/)
add_dependencies(${PROJECT_NAME} copyDemoResources)
