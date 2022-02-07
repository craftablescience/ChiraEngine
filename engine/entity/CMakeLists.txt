include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/camera/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/gui/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/logic/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/model/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/physics/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/root/CMakeLists.txt)

list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/entity.cpp
        ${CMAKE_CURRENT_LIST_DIR}/entity.h)
