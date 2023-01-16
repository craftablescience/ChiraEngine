#include "ModelScene.h"

#include <entity/root/Frame.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include "../camera/EditorCamera.h"
#include <core/Engine.h>
#include <entity/model/MeshDynamic.h>

using namespace chira;

ModelScene::ModelScene() 
    : EngineView() {
    frame->setBackgroundColor(ColorRGB{0.15f});

    auto camera = new EditorCamera{CameraProjectionMode::PERSPECTIVE, 120.f};
    camera->translate({-6.f * sqrtf(3.f), 6, 0});
    camera->setPitch(30.f);
    camera->setYaw(270.f);
    // camera->setName("EDITOR_CAMERA");
    frame->addChild(camera);
    frame->setCamera(camera);

    // OPTIMIZE: replace this multi cube mesh grid with a single textured plane grid
    // IDEA: the grid texture I made for Chisel could be reused here!
    auto grid = new MeshDynamic{"EDITOR_GRID"};
    auto gridMesh = grid->getMesh();
    gridMesh->setMaterial(CHIRA_GET_MATERIAL("MaterialUntextured", "file://materials/unlit.json"));
    for (int i = -5; i <= 5; i++) {
        gridMesh->addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.025f});
        gridMesh->addCube(Vertex{{0, 0, i}}, {10.025f, 0.025f, 0.025f});
    }
    gridMesh->addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
    gridMesh->addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
    gridMesh->addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});
    frame->addChild(grid);
}

void ModelScene::renderOverlayContents() {
    // a very nice little collapsable view for our info!
    if (ImGui::BeginChild("evopt_holder", ImVec2(200, ImGui::GetWindowHeight()))) {
        if (ImGui::CollapsingHeader(TRC("ui.engineview.options"))) {
            ImGui::Checkbox(TRC("ui.editor.show_grid"), &showGrid);
            frame->getChild("EDITOR_GRID")->setVisible(showGrid);
            ImGui::Text("Current file: %s", loadedFile.c_str());
        }
        ImGui::EndChild();
    }
}
