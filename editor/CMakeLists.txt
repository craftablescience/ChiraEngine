list(APPEND CHIRA_HEADER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Editor.h
        ${CMAKE_CURRENT_LIST_DIR}/EditorPlugin.h
)

list(APPEND CHIRA_EDITOR_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Editor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/EditorPlugin.cpp
)

if(WIN32)
        list(APPEND CHIRA_EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/platform/windows/icon.rc)
endif()

if(APPLE)
        list(APPEND CHIRA_APPLE_BUNDLE_FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/AppIcon.icns
                ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/Info.plist)
        list(APPEND CHIRA_EDITOR_SOURCES ${CHIRA_APPLE_BUNDLE_FILES})
endif()

# macos bundle for editor
# if(${CMAKE_SYSTEM} MATCHES "Darwin")
#     add_executable(ChiraEditor MACOSX_BUNDLE ${CHIRA_EDITOR_SOURCES})
# else()
        add_executable(ChiraEditor ${CHIRA_EDITOR_SOURCES})
# endif()

apply_optimizations(ChiraEditor)

if(WIN32)
        add_custom_target(
                CopyEditorIcon
                ALL
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/windows/icon.ico ${CMAKE_BINARY_DIR}/resources/platform/windows/icon.ico)
        add_dependencies(ChiraEditor CopyEditorIcon)
endif()

if(APPLE)
        set_target_properties(ChiraEditor PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_FRAMEWORK_IDENTIFIER info.craftablescience.ChiraEditor
                BUNDLE "${CHIRA_APPLE_BUNDLE_FILES}")
endif()

target_link_libraries(ChiraEditor PRIVATE ${PROJECT_NAME})
target_include_directories(ChiraEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/editor)

option(CHIRA_EDITOR_BUILD_INSTALLER "Build an installer for the editor binaries" OFF)
if(CHIRA_EDITOR_BUILD_INSTALLER)
        set(CPACK_PACKAGE_VENDOR "Chira Engine Developers" CACHE STRING "" FORCE)
        set(CPACK_GENERATOR "TGZ" CACHE STRING "" FORCE)
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An open source game engine for Windows, Linux, and macOS." CACHE STRING "" FORCE)
        set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding/steam/avatars/spider.png" CACHE STRING "" FORCE)
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt" CACHE STRING "" FORCE)
        set(CPACK_MONOLITHIC_INSTALL TRUE CACHE BOOL "" FORCE)
        if(WIN32)
                #todo: NSIS or WIX
                #set(CPACK_GENERATOR "WIX" CACHE STRING "" FORCE)
                message(FATAL_ERROR "Sorry, no Windows installer is supported yet.")
        elseif(APPLE)
                set(CPACK_GENERATOR "Bundle" CACHE STRING "" FORCE)
                set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/branding/github/readme_banner.png" CACHE STRING "" FORCE)
                set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE TRUE CACHE BOOL "" FORCE)
                set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/Info.plist CACHE STRING "" FORCE)
                set(CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/macOS/AppIcon.icns CACHE STRING "" FORCE)
        elseif(UNIX)
                #todo: DEB and/or RPM
                message(FATAL_ERROR "Sorry, no Linux installer is supported yet.")
        endif()
        include(CPack)
endif()