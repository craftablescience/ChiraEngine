# AngelScript
include(third_party/angelscript/CMakeLists.txt)

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
add_subdirectory(third_party/glad)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# GLM
add_subdirectory(third_party/glm)

# OpenAL-Soft
set(ALSOFT_EXAMPLES         OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_NO_CONFIG_UTIL   OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS            OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS    OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/openalsoft)

# NLOHMANN_JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(third_party/json)

# STB
include(third_party/stb/CMakeLists.txt)

# IMGUI
list(APPEND ENGINE_SOURCES
        third_party/imgui/imconfig.h
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_demo.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_internal.h
        third_party/imgui/imgui_tables.cpp
        third_party/imgui/imgui_widgets.cpp
        third_party/imgui/imstb_rectpack.h
        third_party/imgui/imstb_textedit.h
        third_party/imgui/imstb_truetype.h
        third_party/imgui/backends/imgui_impl_glfw.cpp
        third_party/imgui/backends/imgui_impl_glfw.h
        third_party/imgui/backends/imgui_impl_opengl3.cpp
        third_party/imgui/backends/imgui_impl_opengl3.h)

# BASICGAMEENGINE
include(config/CMakeLists.txt)
include(core/CMakeLists.txt)
include(input/CMakeLists.txt)
include(loader/CMakeLists.txt)
include(render/CMakeLists.txt)
include(script/CMakeLists.txt)
include(ui/CMakeLists.txt)
include(utility/CMakeLists.txt)
add_library(${PROJECT_NAME} ${ENGINE_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC third_party/angelscript/include)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/angelscript/addons)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC third_party/stb)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/imgui)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glad glfw glm::glm OpenAL nlohmann_json::nlohmann_json)
