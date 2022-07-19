include(${CMAKE_CURRENT_SOURCE_DIR}/engine/render/material/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/engine/render/mesh/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Light.h
        ${CMAKE_CURRENT_LIST_DIR}/Shader.h
        ${CMAKE_CURRENT_LIST_DIR}/Texture.h
        ${CMAKE_CURRENT_LIST_DIR}/TextureCubemap.h
        ${CMAKE_CURRENT_LIST_DIR}/UBO.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Light.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Shader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Texture.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TextureCubemap.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UBO.cpp)
