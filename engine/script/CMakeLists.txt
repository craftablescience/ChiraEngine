if (${CHIRA_SCRIPT_LANG} MATCHES "AngelScript")
    include(${CMAKE_CURRENT_LIST_DIR}/AS/CMakeLists.txt)
elseif (${CHIRA_SCRIPT_LANG} MATCHES "CSharp")
    include(${CMAKE_CURRENT_LIST_DIR}/Sharp/CMakeLists.txt)
endif ()

list(APPEND CHIRA_ENGINE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/Engine.h
)