list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/ChiraMeshLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/IMeshLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/OBJMeshLoader.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/ChiraMeshLoader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IMeshLoader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/OBJMeshLoader.cpp)
