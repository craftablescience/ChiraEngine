include(${CMAKE_CURRENT_SOURCE_DIR}/utility/math/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/utility/string/CMakeLists.txt)

list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/curlMemoryWriter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/curlMemoryWriter.h
        ${CMAKE_CURRENT_LIST_DIR}/handleObject.h
        ${CMAKE_CURRENT_LIST_DIR}/logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/logger.h
        ${CMAKE_CURRENT_LIST_DIR}/uuidGenerator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/uuidGenerator.h)
