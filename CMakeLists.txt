cmake_minimum_required(VERSION 3.19)
project(ChiraEngineProject)
set(CMAKE_CXX_STANDARD 17)

# Definitions
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()

add_subdirectory(src)

list(APPEND DEMO_SOURCES ${CMAKE_CURRENT_LIST_DIR}/example/demo.cpp)
if(WIN32)
    list(APPEND DEMO_SOURCES ${CMAKE_CURRENT_LIST_DIR}/example/resources/demo/win32/project.rc)
endif()
add_executable(${PROJECT_NAME} ${DEMO_SOURCES})
target_link_libraries(${PROJECT_NAME} ChiraEngine)

# Resources provided by the engine
add_custom_target(copyEngineResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/engine/
        ${CMAKE_BINARY_DIR}/resources/engine/)
add_dependencies(${PROJECT_NAME} copyEngineResources)

# Resources provided by the demo
add_custom_target(copyDemoResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/example/resources/demo/
        ${CMAKE_BINARY_DIR}/resources/demo/)
add_dependencies(${PROJECT_NAME} copyDemoResources)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
