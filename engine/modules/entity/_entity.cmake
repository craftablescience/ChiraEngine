list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Entity.h
        ${CMAKE_CURRENT_LIST_DIR}/Viewport.h
        ${CMAKE_CURRENT_LIST_DIR}/Scene.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Viewport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Scene.cpp)

include_subdirs(component)
