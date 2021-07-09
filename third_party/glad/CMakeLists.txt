list(APPEND GLAD_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/glad.c
        ${CMAKE_CURRENT_LIST_DIR}/include/glad/gl.h
        ${CMAKE_CURRENT_LIST_DIR}/include/KHR/khrplatform.h)
list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/gl.c)