include_subdirs(engine)

list(APPEND CHIRA_TEST_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/TestHelpers.h")

FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "release-1.12.1")
FetchContent_MakeAvailable(googletest)

enable_testing()

set(CHIRA_TEST_NAME "ChiraTest")
add_executable(${CHIRA_TEST_NAME} ${CHIRA_TEST_SOURCES})
target_link_libraries(${CHIRA_TEST_NAME} PUBLIC ${CHIRA_ENGINE_NAME} gtest_main)
target_include_directories(${CHIRA_TEST_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

include(GoogleTest)
gtest_discover_tests(${CHIRA_TEST_NAME})
