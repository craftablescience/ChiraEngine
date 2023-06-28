include(${CMAKE_CURRENT_LIST_DIR}/audio/CMakeLists.txt)

if(CHIRA_USE_DISCORD)
    include(${CMAKE_CURRENT_LIST_DIR}/discord/CMakeLists.txt)
endif()

if(CHIRA_USE_STEAMWORKS)
    include(${CMAKE_CURRENT_LIST_DIR}/steam/CMakeLists.txt)
endif()

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Module.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Module.cpp)
