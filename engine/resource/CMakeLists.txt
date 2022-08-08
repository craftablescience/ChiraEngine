include(${CMAKE_CURRENT_SOURCE_DIR}/engine/resource/provider/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/BinaryResource.h
        ${CMAKE_CURRENT_LIST_DIR}/Properties.h
        ${CMAKE_CURRENT_LIST_DIR}/PropertiesResource.h
        ${CMAKE_CURRENT_LIST_DIR}/Resource.h
        ${CMAKE_CURRENT_LIST_DIR}/ShaderResource.h
        ${CMAKE_CURRENT_LIST_DIR}/StringResource.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/BinaryResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PropertiesResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Resource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ShaderResource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/StringResource.cpp)
