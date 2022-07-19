list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/ISettingsLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/JSONSettingsLoader.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/ISettingsLoader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/JSONSettingsLoader.cpp)
