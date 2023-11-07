list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Font.h
        ${CMAKE_CURRENT_LIST_DIR}/IPanel.h
        ${CMAKE_CURRENT_LIST_DIR}/IViewportPanel.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Font.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IPanel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IViewportPanel.cpp)

include_subdirs(debug)
