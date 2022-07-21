list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Assertions.h
        ${CMAKE_CURRENT_LIST_DIR}/ConCommand.h
        ${CMAKE_CURRENT_LIST_DIR}/ConVar.h
        ${CMAKE_CURRENT_LIST_DIR}/Dialogs.h
        ${CMAKE_CURRENT_LIST_DIR}/FileInputStream.h
        ${CMAKE_CURRENT_LIST_DIR}/HandleObject.h
        ${CMAKE_CURRENT_LIST_DIR}/IFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/Logger.h
        ${CMAKE_CURRENT_LIST_DIR}/Platform.h
        ${CMAKE_CURRENT_LIST_DIR}/SharedPointer.h
        ${CMAKE_CURRENT_LIST_DIR}/String.h
        ${CMAKE_CURRENT_LIST_DIR}/Types.h
        ${CMAKE_CURRENT_LIST_DIR}/UUIDGenerator.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/ConCommand.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ConVar.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Dialogs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileInputStream.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/String.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UUIDGenerator.cpp)
