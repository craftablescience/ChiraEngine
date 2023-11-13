list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/CommandLine.h
        ${CMAKE_CURRENT_LIST_DIR}/FileSystem.h
        ${CMAKE_CURRENT_LIST_DIR}/IModule.h
        ${CMAKE_CURRENT_LIST_DIR}/Platform.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/CommandLine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileSystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IModule.cpp)

include_subdirs(config debug math utility)
