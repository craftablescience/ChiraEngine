# Add hooks
if(CHIRA_USE_DISCORD)
    include_subdirs(discord)
endif()

if(CHIRA_USE_STEAMWORKS)
    include_subdirs(steam)
endif()
