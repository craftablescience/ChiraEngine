cmake_minimum_required(VERSION 3.20)
project(BasicGameEngine)
find_package(OpenGL REQUIRED)
set(CMAKE_CXX_STANDARD 20)

include_directories( ${OPENGL_INCLUDE_DIRS} )
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

add_executable(BasicGameEngine main.cpp)
target_link_libraries(BasicGameEngine ${OPENGL_LIBRARIES} glfw)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()
