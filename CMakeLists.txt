cmake_minimum_required(VERSION 3.19)
project(ChiraEngine)
set(CMAKE_CXX_STANDARD 17)

# Definitions
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()

add_subdirectory(src)
add_subdirectory(example)
