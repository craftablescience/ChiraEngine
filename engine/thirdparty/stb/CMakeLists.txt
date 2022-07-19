# https://github.com/nothings/stb
cmake_minimum_required(VERSION 3.15)
project(stb)
set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h>")
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
