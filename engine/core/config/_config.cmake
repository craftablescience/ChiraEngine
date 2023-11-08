configure_file(${CMAKE_CURRENT_LIST_DIR}/generated/Config.h.in ${CMAKE_CURRENT_LIST_DIR}/generated/Config.h)

include_subdirs(file)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/generated/Config.h
        ${CMAKE_CURRENT_LIST_DIR}/ConEntry.h
        ${CMAKE_CURRENT_LIST_DIR}/Config.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/ConEntry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Config.cpp)
