cmake_minimum_required(VERSION 3.16)
project(glad)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

if(NOT DEFINED GLAD_GL_VERSION)
    message(FATAL_ERROR "Must set GLAD_GL_VERSION variable to 40, 41, or 43!")
endif()

add_library(${PROJECT_NAME} STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/${GLAD_GL_VERSION}/include/glad/glversion.h
        ${CMAKE_CURRENT_SOURCE_DIR}/${GLAD_GL_VERSION}/include/glad/gl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/${GLAD_GL_VERSION}/src/gl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/${GLAD_GL_VERSION}/include/KHR/khrplatform.h)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/${GLAD_GL_VERSION}/include)
