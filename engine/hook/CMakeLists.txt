if(CHIRA_BUILD_WITH_DISCORD)
    list(APPEND ENGINE_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/discordRPC.cpp
            ${CMAKE_CURRENT_LIST_DIR}/discordRPC.h)
endif()

if(CHIRA_BUILD_WITH_STEAMWORKS)
    list(APPEND ENGINE_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/steamAPI.cpp
            ${CMAKE_CURRENT_LIST_DIR}/steamAPI.h)
endif()
