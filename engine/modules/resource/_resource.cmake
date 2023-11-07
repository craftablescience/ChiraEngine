list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/BinaryResource.h
        ${CMAKE_CURRENT_LIST_DIR}/JSONResource.h
        ${CMAKE_CURRENT_LIST_DIR}/Resource.h
        ${CMAKE_CURRENT_LIST_DIR}/StringResource.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/BinaryResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/JSONResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Resource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/StringResource.cpp)
