list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/abstractResourceProvider.h
        ${CMAKE_CURRENT_LIST_DIR}/filesystemResourceProvider.cpp
        ${CMAKE_CURRENT_LIST_DIR}/filesystemResourceProvider.h)

if(CHIRA_BUILD_WITH_NETWORKING)
    list(APPEND ENGINE_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/internetResourceProvider.cpp
            ${CMAKE_CURRENT_LIST_DIR}/internetResourceProvider.h)
endif()
