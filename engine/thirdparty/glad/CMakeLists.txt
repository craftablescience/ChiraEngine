cmake_minimum_required(VERSION 3.15)
project(glad)
set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/gl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
