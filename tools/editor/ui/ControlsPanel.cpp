#include "ControlsPanel.h"

#include <fstream>

#include <ImGuizmo.h>

#include <entity/component/CameraComponent.h>
#include <entity/component/MeshComponent.h>
#include <entity/component/MeshDynamicComponent.h>
#include <entity/Entity.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <platforms/Common.h>

using namespace chira;

static void setupKeybinds(TransformComponent& cameraTransform) {
    constexpr float movementSpeed    = 4.f;
    constexpr float mouseSensitivity = 6.f;
    constexpr float scrollModifier   = 10.f;

    Input::KeyEvent::create(Input::Key::SDLK_w, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, 0, -movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000)});
    });
    Input::KeyEvent::create(Input::Key::SDLK_s, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, 0, movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000)});
    });
    Input::KeyEvent::create(Input::Key::SDLK_a, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({-movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0, 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_d, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0, 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_SPACE, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_LSHIFT, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, -movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0});
    });
    Input::MouseEvent::create(Input::Mouse::BUTTON_RIGHT, Input::MouseEventType::CLICKED, [](int, int, uint8_t) {
        Device::setMouseCapturedWindow(Engine::getMainWindow(), true);
    });
    Input::MouseEvent::create(Input::Mouse::BUTTON_RIGHT, Input::MouseEventType::RELEASED, [](int, int, uint8_t) {
        Device::setMouseCapturedWindow(Engine::getMainWindow(), false);
    });
    Input::MouseMotionEvent::create(Input::MouseMotion::MOVEMENT, Input::MouseMotionEventType::NOT_APPLICABLE, [&cameraTransform](int, int, int xRel, int yRel) {
        if (!Device::isMouseCapturedWindow(Engine::getMainWindow()))
            return;

        float xOffset = static_cast<float>(xRel) * mouseSensitivity * (static_cast<float>(Engine::getDeltaTicks()) / 1000);
        float yOffset = static_cast<float>(yRel) * mouseSensitivity * (static_cast<float>(Engine::getDeltaTicks()) / 1000);

        float yaw = glm::degrees(cameraTransform.getYaw());
        yaw -= xOffset;

        float pitch = glm::degrees(cameraTransform.getPitch());
        pitch -= yOffset;
        pitch = std::clamp(pitch, -89.9f, 89.9f);

        cameraTransform.setYaw(glm::radians(yaw));
        cameraTransform.setPitch(glm::radians(pitch));
    });
    Input::MouseMotionEvent::create(Input::MouseMotion::SCROLL, Input::MouseMotionEventType::NOT_APPLICABLE, [&cameraTransform](int x, int y, int, int) {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({
                static_cast<float>(x) * scrollModifier / 40,
                0,
                -static_cast<float>(y) * scrollModifier / 40 // negate for OpenGL
            });
    });
}

ControlsPanel::ControlsPanel(Viewport* viewport)
        : IPanel("Controls", true)
        , scene(viewport->addScene("Editor"))
        , selectedEntity(nullptr)
        , selectedScene(nullptr) {
    auto* camera = this->scene->addEntity("Camera");
    camera->addComponent<CameraComponent>(CameraComponent::ProjectionMode::PERSPECTIVE);

    auto& cameraTransform = camera->getTransform();
    cameraTransform.translate({-6.f * sqrtf(3.f), 6, 0});
    cameraTransform.setPitch(glm::radians(-30.f));
    cameraTransform.setYaw(glm::radians(270.f));

    auto* grid = this->scene->addEntity("Grid");
    this->gridID = grid->getUUID();
    auto& gridMesh = grid->addComponent<MeshDynamicComponent>();
    gridMesh.meshBuilder.setMaterial(CHIRA_GET_MATERIAL("MaterialUntextured", "materials/unlit.json"));
    for (int i = -5; i <= 5; i++) {
        gridMesh.meshBuilder.addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.025f});
        gridMesh.meshBuilder.addCube(Vertex{{0, 0, i}}, {10.025f, 0.025f, 0.025f});
    }
    gridMesh.meshBuilder.addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
    gridMesh.meshBuilder.addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
    gridMesh.meshBuilder.addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});

    setupKeybinds(cameraTransform);
}

void ControlsPanel::preRenderContents() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
            if (ImGui::MenuItem(TRC("ui.menubar.exit"))) // Exit
                Device::queueDestroyWindow(Engine::getMainWindow(), true);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ControlsPanel::renderContents() {
    static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
    static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
    static bool useSnap = false;

    if (ImGui::CollapsingHeader("View", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (this->scene->hasEntity(this->gridID)) {
            ImGui::Checkbox(TRC("ui.editor.show_grid"), &this->showGrid);
            this->scene->getEntity(this->gridID)->setVisible(this->showGrid);
        }
        if (this->scene->hasEntity(this->previewID)) {
            ImGui::Text("%s", this->loadedFile.c_str());
        }
    }

    if (!this->selectedEntity)
        return;

    if (auto* camera = this->scene->getCamera()) {
        static float snap[3] = { 1.f, 1.f, 1.f };

        if (ImGui::CollapsingHeader("Gizmo", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::RadioButton("Translate", (int*) &currentGizmoOperation, ImGuizmo::TRANSLATE);
            ImGui::RadioButton("Rotate", (int*) &currentGizmoOperation, ImGuizmo::ROTATE);
            ImGui::RadioButton("Scale", (int*) &currentGizmoOperation, ImGuizmo::SCALE);
            ImGui::Separator();
            ImGui::RadioButton("Local", (int*) &currentGizmoMode, ImGuizmo::LOCAL);
            ImGui::RadioButton("World", (int*) &currentGizmoMode, ImGuizmo::WORLD);
            ImGui::Checkbox("Snap", &useSnap);
            ImGui::InputFloat3("Snap", snap);
        }

        auto& transform = this->selectedEntity->getTransform();

        const auto view = camera->getView();
        // todo(editor): HACK HACK HACK
        const auto proj = camera->getProjection(Device::getWindowSize(Engine::getMainWindow()));
        glm::mat4 matrix = transform.getMatrix();

        ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
        ImGuizmo::Manipulate(&view[0][0], &proj[0][0], currentGizmoOperation, currentGizmoMode, &matrix[0][0], useSnap ? snap : nullptr);

        transform.setMatrixLocal(matrix);
    }
}

void ControlsPanel::setSelectedEntity(Entity* selected) {
    this->selectedEntity = selected;
}

void ControlsPanel::setSelectedScene(Scene* selected) {
    this->selectedScene = selected;
}
