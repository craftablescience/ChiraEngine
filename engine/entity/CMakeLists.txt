include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/camera/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/model/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/root/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Entity.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Entity.cpp)
