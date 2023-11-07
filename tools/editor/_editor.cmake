include_subdirs(ui)

list(APPEND CHIRA_EDITOR_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/editor.cpp)

set(CHIRA_EDITOR_NAME "editor")
add_tool_executable(${CHIRA_EDITOR_NAME} GUI SOURCES ${CHIRA_EDITOR_SOURCES})
