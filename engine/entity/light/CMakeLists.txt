list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/DirectionalLight.h
        ${CMAKE_CURRENT_LIST_DIR}/LightManager.h
        ${CMAKE_CURRENT_LIST_DIR}/PointLight.h
        ${CMAKE_CURRENT_LIST_DIR}/SpotLight.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/DirectionalLight.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LightManager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PointLight.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SpotLight.cpp)
