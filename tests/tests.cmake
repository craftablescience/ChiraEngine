list(APPEND CHIRA_TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/TestHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/engine/config/ConEntryTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/core/CommandLine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/resource/provider/FilesystemResourceProviderTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/ui/debug/ConsolePanelTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/ConceptsTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/StringTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/TypeStringTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/UUIDGeneratorTest.cpp)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(CHIRA_TEST_NAME "ChiraTest")
add_executable(${CHIRA_TEST_NAME} ${CHIRA_TEST_SOURCES})
target_link_libraries(${CHIRA_TEST_NAME} PUBLIC ${CHIRA_ENGINE_NAME} gtest_main)
target_include_directories(${CHIRA_TEST_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

include(GoogleTest)
gtest_discover_tests(${CHIRA_TEST_NAME})
