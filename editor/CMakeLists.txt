list(APPEND CHIRA_HEADER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Editor.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EngineView.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/ResourceBrowser.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EntityPanel.h
)

list(APPEND CHIRA_EDITOR_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Editor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EngineView.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/ResourceBrowser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EntityPanel.cpp
)
