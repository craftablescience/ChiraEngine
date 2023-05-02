include(${CMAKE_CURRENT_LIST_DIR}/debug/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Font.h
        ${CMAKE_CURRENT_LIST_DIR}/IPanel.h
        ${CMAKE_CURRENT_LIST_DIR}/ILayerPanel.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Font.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IPanel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ILayerPanel.cpp)
