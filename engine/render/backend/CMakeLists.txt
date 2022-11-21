list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/RenderBackendGL.h
        ${CMAKE_CURRENT_LIST_DIR}/RenderBackendNull.h
        ${CMAKE_CURRENT_LIST_DIR}/Renderer.h
        ${CMAKE_CURRENT_LIST_DIR}/RendererTypes.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/RenderBackendGL.cpp
        ${CMAKE_CURRENT_LIST_DIR}/RendererTypes.cpp)
