# Build and copy assembly
add_custom_target(
        ChiraTestAssembly
        ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/ChiraSharp.dll ${CMAKE_CURRENT_LIST_DIR}/ChiraTest/ChiraTest/.mono/assemblies/Debug/ChiraSharp.dll
        COMMAND msbuild ${CMAKE_CURRENT_LIST_DIR}/ChiraTest/ChiraTest.sln
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/ChiraTest/ChiraTest/bin/Debug/ChiraTest.dll ${CMAKE_BINARY_DIR}/ChiraTest.dll
)

list(APPEND CHIRA_TEST_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/MonoRuntimeTest.cpp)