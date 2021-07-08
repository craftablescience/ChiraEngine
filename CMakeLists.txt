cmake_minimum_required(VERSION 3.20)
project(BasicGameEngine)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_executable(BasicGameEngine src/main.cpp)
