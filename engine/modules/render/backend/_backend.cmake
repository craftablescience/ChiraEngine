list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/RenderBackend.h
        ${CMAKE_CURRENT_LIST_DIR}/RenderDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/RenderTypes.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/RenderTypes.cpp)

include_subdirs(api device)
