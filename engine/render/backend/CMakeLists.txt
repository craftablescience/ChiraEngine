#todo: strip backends when not using them

include(${CMAKE_CURRENT_SOURCE_DIR}/engine/render/backend/gl/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/render/backend/null/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/RenderBackend.h
        ${CMAKE_CURRENT_LIST_DIR}/RenderTypes.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/RenderTypes.cpp)
