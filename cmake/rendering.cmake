# makes the required changes between renderers much easier to implement via single lists and variables

macro("render_def" def)
    list(APPEND CHIRA_ENGINE_DEFINITIONS CHIRA_USE_${def})
endmacro()

macro("imgui_platform" backend render)
	list(APPEND IMGUI_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_${backend}.h
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_${render}.h)
    list(APPEND IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_${backend}.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/imgui/backends/imgui_impl_${render}.cpp)
endmacro()