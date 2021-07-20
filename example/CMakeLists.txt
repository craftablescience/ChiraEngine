add_executable(BasicGameEngineDemo demo.cpp)
target_link_libraries(BasicGameEngineDemo BasicGameEngine)

# Resources provided by the engine
add_custom_target(copyEngineResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/resources/basicgameengine/
        ${CMAKE_BINARY_DIR}/example/resources/basicgameengine/)
add_dependencies(BasicGameEngineDemo copyEngineResources)

# Resources provided by the demo
add_custom_target(copyDemoResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${CMAKE_BINARY_DIR}/example/resources/)
add_dependencies(BasicGameEngineDemo copyDemoResources)
