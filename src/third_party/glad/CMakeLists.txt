set(GLAD_EXT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/third_party/glad/src)
set(GLAD_EXT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/third_party/glad/include)
add_library(glad ${GLAD_EXT_INCLUDE_DIR}/glad/gl.h ${GLAD_EXT_SOURCE_DIR}/gl.c ${GLAD_EXT_INCLUDE_DIR}/KHR/khrplatform.h)
target_include_directories(glad PUBLIC ${GLAD_EXT_INCLUDE_DIR})
