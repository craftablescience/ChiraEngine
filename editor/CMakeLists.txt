list(APPEND CHIRA_EDITOR_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Editor.h
        ${CMAKE_CURRENT_LIST_DIR}/EngineView.h
)

list(APPEND CHIRA_EDITOR_SOURCES
        ${CHIRA_EDITOR_HEADERS}
        ${CMAKE_CURRENT_LIST_DIR}/Editor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/EngineView.cpp
)

include(${CMAKE_CURRENT_LIST_DIR}/modeleditor/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/camera/CMakeLists.txt)
