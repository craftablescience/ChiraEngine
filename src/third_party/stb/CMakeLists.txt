# https://github.com/nothings/stb
cmake_minimum_required(VERSION 3.15)
project(stb)

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/stb_image.h)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
