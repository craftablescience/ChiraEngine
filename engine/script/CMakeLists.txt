list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/abstractScriptProvider.h)

if(CHIRA_BUILD_WITH_ANGELSCRIPT)
    list(APPEND ENGINE_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/angelscriptHelpers.h
            ${CMAKE_CURRENT_LIST_DIR}/angelscriptHolder.cpp
            ${CMAKE_CURRENT_LIST_DIR}/angelscriptHolder.h
            ${CMAKE_CURRENT_LIST_DIR}/angelscriptProvider.cpp
            ${CMAKE_CURRENT_LIST_DIR}/angelscriptProvider.h)
endif()
