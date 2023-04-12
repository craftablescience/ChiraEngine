include(${CMAKE_CURRENT_SOURCE_DIR}/engine/entity/component/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Entity.h
        ${CMAKE_CURRENT_LIST_DIR}/Layer.h
        ${CMAKE_CURRENT_LIST_DIR}/Scene.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Layer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Scene.cpp)
