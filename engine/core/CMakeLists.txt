list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Assertions.h
        ${CMAKE_CURRENT_LIST_DIR}/CommandLine.h
        ${CMAKE_CURRENT_LIST_DIR}/Engine.h
        ${CMAKE_CURRENT_LIST_DIR}/Logger.h
        ${CMAKE_CURRENT_LIST_DIR}/Platform.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/CommandLine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Engine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Logger.cpp)
