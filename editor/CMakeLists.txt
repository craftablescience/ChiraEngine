list(APPEND CHIRA_EDITOR_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Editor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Editor.h)

if (CHIRA_USE_QT)
    list(APPEND CHIRA_EDITOR_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Panels/resourcebrowser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Panels/resourcebrowser.h
        ${CMAKE_CURRENT_LIST_DIR}/Panels/resourcebrowser.ui
        ${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mainwindow.h
        ${CMAKE_CURRENT_LIST_DIR}/mainwindow.ui)
endif()
