list(APPEND ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/image/abstractImage.h
        ${CMAKE_CURRENT_LIST_DIR}/image/image.cpp
        ${CMAKE_CURRENT_LIST_DIR}/image/image.h
        ${CMAKE_CURRENT_LIST_DIR}/mesh/abstractMeshLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/mesh/objMeshLoader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mesh/objMeshLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/mesh/primitiveMeshLoader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mesh/primitiveMeshLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/settings/abstractSettingsLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/settings/abstractFileSettingsLoader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/settings/abstractFileSettingsLoader.h
        ${CMAKE_CURRENT_LIST_DIR}/settings/jsonSettingsLoader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/settings/jsonSettingsLoader.h)
