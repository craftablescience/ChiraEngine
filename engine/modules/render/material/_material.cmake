list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/MaterialCubemap.h
        ${CMAKE_CURRENT_LIST_DIR}/MaterialFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/MaterialFrameBuffer.h
        ${CMAKE_CURRENT_LIST_DIR}/MaterialPhong.h
        ${CMAKE_CURRENT_LIST_DIR}/MaterialTextured.h
        ${CMAKE_CURRENT_LIST_DIR}/MaterialUntextured.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/MaterialCubemap.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MaterialFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MaterialFrameBuffer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MaterialPhong.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MaterialTextured.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MaterialUntextured.cpp)
