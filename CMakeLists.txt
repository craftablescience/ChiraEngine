# Note: If you want to use Chira Engine in your own project, do not include
#       this CMake file, or add the root directory through add_subdirectory.
#       Use add_subdirectory(path/to/ChiraEngine/src) instead.
#       This buildscript builds the editor, and the engine & editor tests.
cmake_minimum_required(VERSION 3.15)
project(ChiraEngineGui)
set(CMAKE_CXX_STANDARD 17)

# You should add this to your own CMake to load Chira Engine's dependencies
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)

# Tests
option(CHIRA_BUILD_TESTS "Run Chira Engine's built-in tests" ON)
if(CHIRA_BUILD_TESTS AND (NOT MSVC))
    # For some reason Visual Studio doesn't like this
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/src/CMakeLists.txt)
endif()

add_subdirectory(src)

include(${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt)
if(WIN32)
    list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/editor/win32/project.rc)
endif()
add_executable(${PROJECT_NAME} ${EDITOR_SOURCES})
target_link_libraries(${PROJECT_NAME} ChiraEngine ${OPENGL_LIBRARIES})

if(CHIRA_BUILD_TESTS AND (NOT MSVC))
    add_executable(ChiraTest ${TEST_SOURCES})
    target_include_directories(ChiraTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(ChiraTest PUBLIC ChiraEngine ${OPENGL_LIBRARIES} gtest_main)
    include(GoogleTest)
    gtest_discover_tests(ChiraTest)
endif()

# Compiling for Windows requires the resources be copied before the RC file is compiled
# However, I'd like to avoid making a custom target, perhaps someday the Linux method can be used for both platforms
if(WIN32)
    # Resource files
    add_custom_target(copyResources ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/editor ${CMAKE_BINARY_DIR}/resources/editor)
    add_dependencies(${PROJECT_NAME} copyResources)
else()
    # Resource files
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/editor ${CMAKE_BINARY_DIR}/resources/editor)
endif()

# Copy engine resources for tests (again...)
if(CHIRA_BUILD_TESTS AND (NOT MSVC))
    add_custom_command(TARGET ChiraTest PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/engine/ ${CMAKE_BINARY_DIR}/resources/engine/)
    add_custom_command(TARGET ChiraTest PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/bin/ ${CMAKE_BINARY_DIR})
endif()
