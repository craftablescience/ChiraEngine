# Add OS-specific libraries
if(APPLE)
    find_library(CORE_LIB CoreFoundation)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES ${CORE_LIB})
endif()

# Basic ImGui sources
list(APPEND IMGUI_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imconfig.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imgui.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imgui_internal.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imstb_rectpack.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imstb_textedit.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imstb_truetype.h
        # Plus ImGuiColorTextEdit for convenience
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui-colortextedit/TextEditor.h
        # Plus ImGui-Filebrowser for convenience
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui-filebrowser/imfilebrowser.h
        # Plus ImGuizmo for convenience
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/GraphEditor.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImCurveEdit.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImGradient.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImGuizmo.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImSequencer.h
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImZoomSlider.h)
list(APPEND IMGUI_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imgui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui/imgui_widgets.cpp
        # Plus ImGuiColorTextEdit for convenience
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui-colortextedit/TextEditor.cpp
        # Plus ImGuizmo for convenience
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/GraphEditor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImCurveEdit.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImGradient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImGuizmo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo/ImSequencer.cpp)
list(APPEND CHIRA_ENGINE_DEFINITIONS IMGUI_DISABLE_OBSOLETE_FUNCTIONS)

# Add device backends
if(CHIRA_RENDER_DEVICE STREQUAL "SDL")
    # SDL2
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/sdl2)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/sdl2/include)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES SDL2::SDL2)

    list(APPEND IMGUI_HEADERS
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_sdl2.h)
    list(APPEND IMGUI_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_sdl2.cpp)
    list(APPEND IMGUI_INCLUDE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/sdl2/include)
    list(APPEND IMGUI_LINK_LIBRARIES SDL2::SDL2)
elseif(CHIRA_RENDER_DEVICE STREQUAL "HEADLESS")
    # todo(render): headless mode
    message(FATAL_ERROR "Headless mode is not implemented yet!")
else()
    message(FATAL_ERROR "Unrecognized render device ${CHIRA_RENDER_DEVICE}!")
endif()

# Figure out what render backend we are using
if(CHIRA_RENDER_BACKEND STREQUAL "AUTO")
    if(WIN32 OR UNIX)
        set(CHIRA_RENDER_BACKEND "GL43" CACHE STRING "" FORCE)
    elseif(APPLE)
        set(CHIRA_RENDER_BACKEND "GL41" CACHE STRING "" FORCE)
    else()
        message(WARNING "Current platform has no default render backend! Defaulting to GL41 for max compatability...")
        set(CHIRA_RENDER_BACKEND "GL41" CACHE STRING "" FORCE)
    endif()
endif()
message(STATUS "Setting render backend to ${CHIRA_RENDER_BACKEND}.")

# Set up that backend
if((CHIRA_RENDER_BACKEND STREQUAL "GL40") OR (CHIRA_RENDER_BACKEND STREQUAL "GL41") OR (CHIRA_RENDER_BACKEND STREQUAL "GL43"))
    # macOS: OpenGL 4.0, 4.1
    # Windows and Linux: OpenGL 4.0, 4.1, 4.3
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_RENDER_BACKEND_GL)
    if(APPLE AND CHIRA_RENDER_BACKEND STREQUAL "GL43")
        message(WARNING "GL43 is not supported on Apple! Falling back to GL41...")
        set(CHIRA_RENDER_BACKEND "GL41")
    endif()

    if(CHIRA_RENDER_BACKEND STREQUAL "GL43")
        set(GLAD_GL_VERSION "43")
        list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_RENDER_BACKEND_GL43)
    elseif(CHIRA_RENDER_BACKEND STREQUAL "GL41")
        set(GLAD_GL_VERSION "41")
        list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_RENDER_BACKEND_GL41)
    elseif(CHIRA_RENDER_BACKEND STREQUAL "GL40")
        set(GLAD_GL_VERSION "40")
        list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_RENDER_BACKEND_GL40)
    endif()

    # GLAD
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glad)
    list(APPEND CHIRA_ENGINE_DEFINITIONS IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glad/include)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glad)

    # Add ImGui platform
    list(APPEND IMGUI_HEADERS
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_opengl3.h)
    list(APPEND IMGUI_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_opengl3.cpp)
elseif(CHIRA_RENDER_BACKEND STREQUAL "SDLRENDERER")
    message(WARNING "Render backend set to SDLRENDERER. Many rendering features will be unavailable! Only use this if necessary!")
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_RENDER_BACKEND_SDLRENDERER)

    # Add ImGui platform
    list(APPEND IMGUI_HEADERS
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_sdlrenderer.h)
    list(APPEND IMGUI_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_sdlrenderer.cpp)
else()
    message(FATAL_ERROR "Unrecognized render backend ${CHIRA_RENDER_BACKEND_OVERRIDE}")
endif()

# IMGUI
list(APPEND IMGUI_SOURCES ${IMGUI_HEADERS})
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui
        ${IMGUI_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE ${IMGUI_LINK_LIBRARIES})

list(APPEND CHIRA_ENGINE_INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui-colortextedit
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imgui-filebrowser
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/imguizmo)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES imgui)

# CEREAL
set(JUST_INSTALL_CEREAL ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/cereal)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/cereal/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES cereal::cereal)

# DISCORD
if(CHIRA_USE_DISCORD)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/discord)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_DISCORD)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/discord/include)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES discord-rpc)
endif()

# FMT
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/fmt)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/fmt/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES fmt::fmt)

# EnTT
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/entt)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/entt/src)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES EnTT::EnTT)

# GLM
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm)
target_compile_definitions(glm INTERFACE GLM_FORCE_SILENT_WARNINGS)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glm)

# LIBLOADER
set(LIBLOADER_BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/libloader)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/libloader/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES LibLoader)

# LUA
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/lua)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES lua::lua)

# MAGIC_ENUM
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/magic_enum)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/magic_enum/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES magic_enum)

# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/json)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES nlohmann_json::nlohmann_json)

# SOL2
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/sol2)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/sol2/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES sol2::sol2)

# SOLOUD
if(BUILD_SHARED_LIBS)
    set(SOLOUD_LIBMODPLUG_SUPPORT ON CACHE BOOL "" FORCE)
else()
    set(SOLOUD_LIBMODPLUG_SUPPORT_STATIC ON CACHE BOOL "" FORCE)
endif()
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/soloud/contrib)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/soloud/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES soloud)

# STB
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/stb)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/stb)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stb)

# STEAMWORKS
if(CHIRA_USE_STEAMWORKS)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_STEAMWORKS)
endif()

# STDUUID
set(UUID_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/stduuid)
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/thirdparty/stduuid/include)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stduuid)

# CHIRAENGINE
include(${CMAKE_CURRENT_LIST_DIR}/config/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/core/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/entity/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/i18n/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/input/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/loader/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/math/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/plugin/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/render/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/resource/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/script/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ui/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utility/CMakeLists.txt)

list(APPEND CHIRA_ENGINE_SOURCES ${CHIRA_ENGINE_HEADERS})
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})

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
