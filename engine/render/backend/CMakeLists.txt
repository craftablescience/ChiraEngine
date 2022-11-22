list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/RenderBackendGL.h
# todo(render): make a null backend for testing
#       ${CMAKE_CURRENT_LIST_DIR}/RenderBackendNull.h
        ${CMAKE_CURRENT_LIST_DIR}/Renderer.h
        ${CMAKE_CURRENT_LIST_DIR}/RendererTypes.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/RenderBackendGL.cpp
        ${CMAKE_CURRENT_LIST_DIR}/RendererTypes.cpp)
