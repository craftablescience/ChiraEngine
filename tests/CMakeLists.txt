list(APPEND CHIRA_TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/TestHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/engine/config/ConEntryTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/core/CommandLine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/event/EventsTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/reflect/Props.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/resource/provider/FilesystemResourceProviderTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/script/AngelScriptHelpersTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/script/AngelScriptHolderTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/ui/debug/ConsolePanelTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/Concepts.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/StringTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/UUIDGeneratorTest.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(ChiraTest ${CHIRA_TEST_SOURCES})
target_link_libraries(ChiraTest PUBLIC ${PROJECT_NAME} gtest_main)
target_include_directories(ChiraTest PRIVATE ${CMAKE_CURRENT_LIST_DIR})

include(GoogleTest)
gtest_discover_tests(ChiraTest)
