# http://tinyfiledialogs.sourceforge.net
cmake_minimum_required(VERSION 3.15)
project(tinyfiledialogs)
set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs.h)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE comdlg32 ole32)
endif()
