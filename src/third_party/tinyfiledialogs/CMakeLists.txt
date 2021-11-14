# http://tinyfiledialogs.sourceforge.net
cmake_minimum_required(VERSION 3.15)
project(tinyfiledialogs)

add_library(${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs.c
        ${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs.h)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC comdlg32 ole32)
endif()
