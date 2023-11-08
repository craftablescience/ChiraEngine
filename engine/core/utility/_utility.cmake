list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/AbstractFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/Concepts.h
        ${CMAKE_CURRENT_LIST_DIR}/DependencyGraph.h
        ${CMAKE_CURRENT_LIST_DIR}/MacroHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/NoCopyOrMove.h
        ${CMAKE_CURRENT_LIST_DIR}/Serial.h
        ${CMAKE_CURRENT_LIST_DIR}/SharedPointer.h
        ${CMAKE_CURRENT_LIST_DIR}/String.h
        ${CMAKE_CURRENT_LIST_DIR}/Tuple.h
        ${CMAKE_CURRENT_LIST_DIR}/Types.h
        ${CMAKE_CURRENT_LIST_DIR}/TypeString.h
        ${CMAKE_CURRENT_LIST_DIR}/UUIDGenerator.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/String.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UUIDGenerator.cpp)
