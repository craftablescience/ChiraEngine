if((CHIRA_RENDER_BACKEND STREQUAL "GL40") OR (CHIRA_RENDER_BACKEND STREQUAL "GL41") OR (CHIRA_RENDER_BACKEND STREQUAL "GL43"))
    list(APPEND CHIRA_ENGINE_HEADERS
            ${CMAKE_CURRENT_LIST_DIR}/DeviceGL.h)
    list(APPEND CHIRA_ENGINE_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/DeviceGL.cpp)
elseif(CHIRA_RENDER_BACKEND STREQUAL "SDLRENDERER")
    list(APPEND CHIRA_ENGINE_HEADERS
            ${CMAKE_CURRENT_LIST_DIR}/DeviceSDL.h)
    list(APPEND CHIRA_ENGINE_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/DeviceSDL.cpp)
endif()