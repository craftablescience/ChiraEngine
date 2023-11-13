# Set asset path
set(CHIRA_DEVELOPER_ASSET_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets" CACHE FILEPATH "" FORCE)

# Include sources (order is important, and exclude tests)
include_subdirs(thirdparty core modules platforms)

# Create library
list(APPEND CHIRA_ENGINE_SOURCES ${CHIRA_ENGINE_HEADERS})

# Add the engine folder and the modules subfolder
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/modules")

set(CHIRA_ENGINE_NAME "engine")
add_library(${CHIRA_ENGINE_NAME} STATIC ${CHIRA_ENGINE_SOURCES})
apply_optimizations(${CHIRA_ENGINE_NAME})

if(CHIRA_BUILD_WITH_PCH)
    target_precompile_headers(${CHIRA_ENGINE_NAME} PRIVATE ${CHIRA_ENGINE_HEADERS})
endif()

target_compile_definitions(${CHIRA_ENGINE_NAME} PUBLIC
        ${CHIRA_ENGINE_DEFINITIONS})
target_include_directories(${CHIRA_ENGINE_NAME} PUBLIC
        ${CHIRA_ENGINE_INCLUDE_DIRS})
target_link_libraries(${CHIRA_ENGINE_NAME} PUBLIC
        ${CHIRA_ENGINE_LINK_LIBRARIES})

# Add tests
if(CHIRA_BUILD_TESTS)
    include_subdirs(tests)
endif()
