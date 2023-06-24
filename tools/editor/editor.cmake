include(${CMAKE_CURRENT_LIST_DIR}/ui/CMakeLists.txt)
list(APPEND CHIRA_EDITOR_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/editor.cpp)

if(APPLE)
    list(APPEND CHIRA_EDITOR_APPLE_BUNDLE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/AppIcon.icns
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/Info.plist)
    list(APPEND CHIRA_EDITOR_SOURCES ${CHIRA_EDITOR_APPLE_BUNDLE_FILES})
endif()

set(CHIRA_EDITOR_NAME "editor")
add_tool_executable(${CHIRA_EDITOR_NAME} GUI SOURCES ${CHIRA_EDITOR_SOURCES})

if(APPLE)
    set_target_properties(${CHIRA_EDITOR_NAME} PROPERTIES
            MACOSX_BUNDLE ON
            MACOSX_FRAMEWORK_IDENTIFIER info.craftablescience.ChiraEngine
            BUNDLE "${CHIRA_EDITOR_APPLE_BUNDLE_FILES}")
endif()
