include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/camera/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/imgui/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/model/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/physics/CMakeLists.txt)

list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/entity.cpp
        ${CMAKE_CURRENT_LIST_DIR}/entity.h
        ${CMAKE_CURRENT_LIST_DIR}/root.cpp
        ${CMAKE_CURRENT_LIST_DIR}/root.h)
