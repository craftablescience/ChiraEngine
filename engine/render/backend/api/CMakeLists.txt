if (${CHIRA_RENDER_BACKEND} MATCHES "OpenGL")
    list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/BackendGL.h)
    list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/BackendGL.cpp)

elseif (${CHIRA_RENDER_BACKEND} MATCHES "DirectX")
    message(WARNING "DirectX rendering is highly experimental! crashes and bugs may occur!")
    list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/BackendDX.h)
    list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/BackendDX.cpp)

endif()