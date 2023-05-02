include(${CMAKE_CURRENT_LIST_DIR}/ui/CMakeLists.txt)
list(APPEND CHIRA_EDITOR_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Editor.cpp)

if(WIN32)
    list(APPEND CHIRA_EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/windows/app.rc)
endif()
if(APPLE)
    list(APPEND CHIRA_APPLE_BUNDLE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/AppIcon.icns
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/Info.plist)
    list(APPEND CHIRA_EDITOR_SOURCES ${CHIRA_APPLE_BUNDLE_FILES})
endif()

set(CHIRA_EDITOR_NAME "editor")
add_executable(${CHIRA_EDITOR_NAME} ${CHIRA_EDITOR_SOURCES})
apply_optimizations(${CHIRA_EDITOR_NAME})

if(APPLE)
    set_target_properties(${CHIRA_EDITOR_NAME} PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_FRAMEWORK_IDENTIFIER info.craftablescience.ChiraEngine
            BUNDLE "${CHIRA_APPLE_BUNDLE_FILES}")
endif()
target_link_libraries(${CHIRA_EDITOR_NAME} PRIVATE ${CHIRA_ENGINE_NAME})
target_include_directories(${CHIRA_EDITOR_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

if(CHIRA_BUILD_EDITOR_INSTALLER)
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
        set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/Info.plist CACHE STRING "" FORCE)
        set(CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/platform/macOS/AppIcon.icns CACHE STRING "" FORCE)
    elseif(UNIX)
        #todo: DEB and/or RPM
        message(FATAL_ERROR "Sorry, no Linux installer is supported yet.")
    endif()
    include(CPack)
endif()
