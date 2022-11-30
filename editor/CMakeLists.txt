list(APPEND CHIRA_HEADER_SOURCES
        #${CMAKE_CURRENT_LIST_DIR}/resource/IconResource.h
        
        ${CMAKE_CURRENT_LIST_DIR}/Editor.h
        ${CMAKE_CURRENT_LIST_DIR}/ResManager.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EngineView.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/CodeView.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/ResourceBrowser.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EntityPanel.h
        ${CMAKE_CURRENT_LIST_DIR}/editorui/ToolsPanel.h
        ${CMAKE_CURRENT_LIST_DIR}/camera/EditorCamera.h
)

list(APPEND CHIRA_EDITOR_SOURCES
        #${CMAKE_CURRENT_LIST_DIR}/resource/IconResource.cpp
        
        ${CMAKE_CURRENT_LIST_DIR}/Editor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ResManager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EngineView.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/CodeView.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/ResourceBrowser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/EntityPanel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/editorui/ToolsPanel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/camera/EditorCamera.cpp
)
