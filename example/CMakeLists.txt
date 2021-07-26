list(APPEND DEMO_SOURCES ${CMAKE_CURRENT_LIST_DIR}/demo.cpp)

if(WIN32)
    list(APPEND DEMO_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/demo/win32/project.rc)
endif()

add_executable(ChiraEngineDemo ${DEMO_SOURCES})
target_link_libraries(ChiraEngineDemo ChiraEngine)

# Resources provided by the engine
add_custom_target(copyEngineResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/resources/engine/
        ${CMAKE_BINARY_DIR}/example/resources/engine/)
add_dependencies(ChiraEngineDemo copyEngineResources)

# Resources provided by the demo
add_custom_target(copyDemoResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${CMAKE_BINARY_DIR}/example/resources/)
add_dependencies(ChiraEngineDemo copyDemoResources)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
