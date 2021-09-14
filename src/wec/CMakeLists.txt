include(${CMAKE_CURRENT_SOURCE_DIR}/wec/component/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/wec/entity/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/wec/world/CMakeLists.txt)

list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/wecManager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/wecManager.h)
