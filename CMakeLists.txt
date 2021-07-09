cmake_minimum_required(VERSION 3.20)
project(BasicGameEngine)
find_package(OpenGL REQUIRED)
set(CMAKE_CXX_STANDARD 20)

#GLAD
include_directories(third_party/glad/include)
include(third_party/glad/CMakeLists.txt)

# GLFW
include_directories( ${OPENGL_INCLUDE_DIRS} )
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(third_party/json)

# BASICGAMEENGINE
include(src/config/CMakeLists.txt)
include(src/core/CMakeLists.txt)
include(src/input/CMakeLists.txt)
include(src/shader/CMakeLists.txt)
include(src/utility/CMakeLists.txt)
add_executable(BasicGameEngine example/demo.cpp ${ENGINE_SOURCES})
target_link_libraries(BasicGameEngine ${OPENGL_LIBRARIES} glfw nlohmann_json::nlohmann_json)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()
