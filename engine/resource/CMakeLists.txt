include(${CMAKE_CURRENT_SOURCE_DIR}/engine/resource/provider/CMakeLists.txt)

list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/binaryResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/binaryResource.h
        ${CMAKE_CURRENT_LIST_DIR}/fontResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fontResource.h
        ${CMAKE_CURRENT_LIST_DIR}/propertiesResource.h
        ${CMAKE_CURRENT_LIST_DIR}/resource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/resource.h
        ${CMAKE_CURRENT_LIST_DIR}/shaderResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/shaderResource.h
        ${CMAKE_CURRENT_LIST_DIR}/stringResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/stringResource.h
        ${CMAKE_CURRENT_LIST_DIR}/textureResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/textureResource.h)
