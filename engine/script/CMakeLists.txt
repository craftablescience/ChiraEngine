include(${CMAKE_CURRENT_LIST_DIR}/bindings/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CHIRA_ASBINDINGS_HEADERS}
        ${CMAKE_CURRENT_LIST_DIR}/ASBindings.h
        ${CMAKE_CURRENT_LIST_DIR}/AngelScriptHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/AngelScriptHolder.h
        ${CMAKE_CURRENT_LIST_DIR}/AngelScriptVM.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CHIRA_ASBINDINGS_SOURCES}
        ${CMAKE_CURRENT_LIST_DIR}/AngelScriptHolder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/AngelScriptVM.cpp)
