list(APPEND CHIRA_MATH_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Axis.h
        ${CMAKE_CURRENT_LIST_DIR}/Color.h
        ${CMAKE_CURRENT_LIST_DIR}/Matrix.h
        ${CMAKE_CURRENT_LIST_DIR}/Types.h
        ${CMAKE_CURRENT_LIST_DIR}/Vertex.h)

list(APPEND CHIRA_MATH_SOURCES
        ${CHIRA_MATH_HEADERS}
        ${CMAKE_CURRENT_LIST_DIR}/Color.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Vertex.cpp)

add_library(mathlib SHARED ${CHIRA_MATH_SOURCES})
target_link_libraries(mathlib PUBLIC nlohmann_json::nlohmann_json glm)

list(APPEND CHIRA_ENGINE_LINK_LIBRARIES mathlib)