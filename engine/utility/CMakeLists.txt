include(${CMAKE_CURRENT_SOURCE_DIR}/engine/utility/debug/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/utility/gui/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/utility/math/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/utility/memory/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/utility/string/CMakeLists.txt)

list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/aabbObject.h
        ${CMAKE_CURRENT_LIST_DIR}/dialogs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/dialogs.h
        ${CMAKE_CURRENT_LIST_DIR}/handleObject.h
        ${CMAKE_CURRENT_LIST_DIR}/logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/logger.h
        ${CMAKE_CURRENT_LIST_DIR}/platform.h
        ${CMAKE_CURRENT_LIST_DIR}/uuidGenerator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/uuidGenerator.h)
