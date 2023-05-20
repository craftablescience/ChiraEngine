// Resources Used:
// https://antongerdelan.net/opengl/d3d11.html
// https://www.prowaretech.com/articles/current/windows/application-programming-interface/basic-directx-11-example
// https://github.com/walbourn/directx-sdk-samples
// https://learn.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews

#include "BackendDX11.h"

#include <cstddef>
#include <map>
#include <stack>
#include <string>

#include <imgui.h>
#include <ImGuizmo.h>
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_dx11.h>

#include <d3d11.h>       // D3D interface
#include <dxgi.h>        // DirectX driver interface
#include <d3dcompiler.h> // shader compiler

#include <core/Assertions.h>
#include <core/Logger.h>

using namespace chira;

CHIRA_CREATE_LOG(DX11);

enum class RenderMode {
    CULL_FACE,
    DEPTH_TEST,
    TEXTURE_CUBE_MAP_SEAMLESS,
};

void Renderer::startImGuiFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);
}

void Renderer::endImGuiFrame() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::destroyImGui() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}