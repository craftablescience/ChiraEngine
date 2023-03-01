list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/AbstractFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/Concepts.h
        ${CMAKE_CURRENT_LIST_DIR}/SharedPointer.h
        ${CMAKE_CURRENT_LIST_DIR}/String.h
        ${CMAKE_CURRENT_LIST_DIR}/Types.h
        ${CMAKE_CURRENT_LIST_DIR}/UUIDGenerator.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/String.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UUIDGenerator.cpp)
