cmake_minimum_required(VERSION 3.15)
project(ChiraEngineGui)
set(CMAKE_CXX_STANDARD 17)

# Definitions
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
    if(UNIX OR MINGW)
        # On UNIX, build with debug friendly optimizations and debug symbols
        # On windows, just disable optimizations entirely
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")
    elseif(MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Od")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od")
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
    # Build with max optimizations and don't omit stack ptr for debugging reasons
    if(UNIX OR MINGW)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fno-omit-frame-pointer")
    elseif(MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
    endif()
endif()

if(UNIX OR MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")
elseif(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
endif()

add_subdirectory(src)

include(${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt)
if(WIN32)
    list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/editor/win32/project.rc)
endif()
add_executable(${PROJECT_NAME} ${EDITOR_SOURCES})
target_link_libraries(${PROJECT_NAME} ChiraEngine ${OPENGL_LIBRARIES})

# Resource files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/engine/
        ${CMAKE_BINARY_DIR}/resources/engine/)
