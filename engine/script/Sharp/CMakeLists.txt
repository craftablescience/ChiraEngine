# build and copy assembly
add_custom_target(
        ChiraSharpAssembly
        ALL
        COMMAND msbuild ${CMAKE_CURRENT_LIST_DIR}/ChiraSharp/ChiraSharp.sln
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/ChiraSharp/ChiraSharp/bin/Debug/ChiraSharp.dll ${CMAKE_BINARY_DIR}/ChiraSharp.dll
)

list(APPEND CHIRA_ENGINE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/MonoRuntime.h)

list(APPEND CHIRA_ENGINE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/MonoRuntime.cpp)