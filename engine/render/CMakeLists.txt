include(${CMAKE_CURRENT_SOURCE_DIR}/engine/render/material/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/render/mesh/CMakeLists.txt)

list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/light.cpp
        ${CMAKE_CURRENT_LIST_DIR}/light.h
        ${CMAKE_CURRENT_LIST_DIR}/shader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/shader.h
        ${CMAKE_CURRENT_LIST_DIR}/texture.cpp
        ${CMAKE_CURRENT_LIST_DIR}/texture.h
        ${CMAKE_CURRENT_LIST_DIR}/textureCubemap.cpp
        ${CMAKE_CURRENT_LIST_DIR}/textureCubemap.h
        ${CMAKE_CURRENT_LIST_DIR}/ubo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ubo.h)
