# Add OS-specific libraries
if(APPLE)
    find_library(CORE_LIB CoreFoundation)
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES ${CORE_LIB})
endif()

# Basic ImGui sources
list(APPEND IMGUI_HEADERS
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imconfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.h"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_internal.h"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imstb_rectpack.h"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imstb_textedit.h"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imstb_truetype.h"
        # Plus ImGuiColorTextEdit for convenience
        "${CMAKE_CURRENT_LIST_DIR}/imgui-colortextedit/TextEditor.h"
        # Plus ImGui-Filebrowser for convenience
        "${CMAKE_CURRENT_LIST_DIR}/imgui-filebrowser/imfilebrowser.h"
        # Plus ImGuizmo for convenience
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/GraphEditor.h"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImCurveEdit.h"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImGradient.h"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImGuizmo.h"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImSequencer.h"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImZoomSlider.h")
list(APPEND IMGUI_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_demo.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp"
        # Plus ImGuiColorTextEdit for convenience
        "${CMAKE_CURRENT_LIST_DIR}/imgui-colortextedit/TextEditor.cpp"
        # Plus ImGuizmo for convenience
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/GraphEditor.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImCurveEdit.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImGradient.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImGuizmo.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo/ImSequencer.cpp")
list(APPEND CHIRA_ENGINE_DEFINITIONS IMGUI_DISABLE_OBSOLETE_FUNCTIONS)

# Add device backends
if(CHIRA_RENDER_DEVICE STREQUAL "SDL2")
    # SDL2
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/sdl2")
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/sdl2/include")
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES SDL2::SDL2)

    list(APPEND IMGUI_HEADERS
            "${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_sdl2.h")
    list(APPEND IMGUI_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_sdl2.cpp")
    list(APPEND IMGUI_INCLUDE_DIRS
            "${CMAKE_CURRENT_LIST_DIR}/sdl2/include")
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

    # Ensure OpenGL exists
    find_package(OpenGL REQUIRED)

    # GLAD
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glad")
    list(APPEND CHIRA_ENGINE_DEFINITIONS IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/glad/include")
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glad)

    # Add ImGui platform
    list(APPEND IMGUI_HEADERS
            "${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_opengl3.h")
    list(APPEND IMGUI_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_opengl3.cpp")
elseif(CHIRA_RENDER_BACKEND STREQUAL "SDLRENDERER")
    message(WARNING "Render backend set to SDLRENDERER. Many rendering features will be unavailable! Only use this if necessary!")
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_RENDER_BACKEND_SDLRENDERER)

    # Add ImGui platform
    list(APPEND IMGUI_HEADERS
            "${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_sdlrenderer.h")
    list(APPEND IMGUI_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_sdlrenderer.cpp")
else()
    message(FATAL_ERROR "Unrecognized render backend ${CHIRA_RENDER_BACKEND_OVERRIDE}")
endif()

# ImGui
list(APPEND IMGUI_SOURCES ${IMGUI_HEADERS})
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/imgui"
        ${IMGUI_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE ${IMGUI_LINK_LIBRARIES})

list(APPEND CHIRA_ENGINE_INCLUDE_DIRS
        "${CMAKE_CURRENT_LIST_DIR}/imgui"
        "${CMAKE_CURRENT_LIST_DIR}/imgui-colortextedit"
        "${CMAKE_CURRENT_LIST_DIR}/imgui-filebrowser"
        "${CMAKE_CURRENT_LIST_DIR}/imguizmo")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES imgui)

# Cereal
set(JUST_INSTALL_CEREAL ON CACHE INTERNAL "")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/cereal")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/cereal/include")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES cereal::cereal)

# Discord
if(CHIRA_USE_DISCORD)
    set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/discord")
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_DISCORD)
    list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/discord/include")
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES discord-rpc)
endif()

# FMT
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/fmt")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/fmt/include")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES fmt::fmt)

# EnTT
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/entt")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/entt/src")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES EnTT::EnTT)

# GLM
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glm")
target_compile_definitions(glm INTERFACE GLM_FORCE_SILENT_WARNINGS)
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES glm)

# LibLoader
set(LIBLOADER_BUILD_TEST OFF CACHE INTERNAL "")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libloader")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/libloader/include")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES LibLoader)

# Lua
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lua")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES lua::lua)

# magic_enum
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/magic_enum")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/magic_enum/include")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES magic_enum)

# nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/json")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES nlohmann_json::nlohmann_json)

# PhysFS
set(PHYSFS_BUILD_TEST OFF CACHE INTERNAL "")
set(PHYSFS_DISABLE_INSTALL OFF CACHE INTERNAL "")
set(PHYSFS_BUILD_DOCS OFF CACHE INTERNAL "")
if(BUILD_SHARED_LIBS)
    set(PHYSFS_BUILD_STATIC OFF CACHE INTERNAL "")
    set(PHYSFS_BUILD_SHARED ON CACHE INTERNAL "")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/physfs")
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES PhysFS::PhysFS)
else()
    set(PHYSFS_BUILD_STATIC ON CACHE INTERNAL "")
    set(PHYSFS_BUILD_SHARED OFF CACHE INTERNAL "")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/physfs")
    list(APPEND CHIRA_ENGINE_LINK_LIBRARIES PhysFS::PhysFS-static)
endif()
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/physfs/src")

# SOL
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/sol2")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/sol2/include")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES sol2::sol2)

# SoLoud
if(BUILD_SHARED_LIBS)
    set(SOLOUD_LIBMODPLUG_SUPPORT ON CACHE INTERNAL "")
else()
    set(SOLOUD_LIBMODPLUG_SUPPORT_STATIC ON CACHE INTERNAL "")
endif()
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/soloud/contrib")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/soloud/include")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES soloud)

# STB
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/stb")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/stb")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stb)

# Steamworks
if(CHIRA_USE_STEAMWORKS)
    include_subdirs(steamworks)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_STEAMWORKS)
endif()

# STDUUID
set(UUID_BUILD_TESTS OFF CACHE INTERNAL "")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/stduuid")
list(APPEND CHIRA_ENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/stduuid/include")
list(APPEND CHIRA_ENGINE_LINK_LIBRARIES stduuid)
