include(${CMAKE_CURRENT_SOURCE_DIR}/engine/ui/debug/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/FontResource.h
        ${CMAKE_CURRENT_LIST_DIR}/FramePanel.h
        ${CMAKE_CURRENT_LIST_DIR}/IPanel.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/FontResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FramePanel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IPanel.cpp)
