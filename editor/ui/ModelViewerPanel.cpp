#include "ModelViewerPanel.h"

#include <fstream>
#include <ImGuizmo.h>
#include <core/Engine.h>
#include <entity/component/CameraComponent.h>
#include <entity/component/MeshComponent.h>
#include <entity/component/MeshDynamicComponent.h>
#include <entity/Entity.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <resource/provider/FilesystemResourceProvider.h>

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

ModelViewerPanel::ModelViewerPanel(Layer* layer)
        : IPanel("Model Viewer", true)
        , scene(layer->addScene("Model Viewer"))
        , selected(nullptr) {
    this->flags |=
            ImGuiWindowFlags_NoTitleBar   |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoResize     |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoBackground ;

    this->modelDialog.SetTitle("Open Resource");
    this->modelDialog.SetTypeFilters({".json"});

    this->folderDialog.SetTitle("Add Resource Folder");

    auto* camera = this->scene->addEntity("Camera");
    camera->addComponent<CameraComponent>(CameraComponent::ProjectionMode::PERSPECTIVE);

    auto& cameraTransform = camera->getTransform();
    cameraTransform.translate({-6.f * sqrtf(3.f), 6, 0});
    cameraTransform.setPitch(glm::radians(-30.f));
    cameraTransform.setYaw(glm::radians(270.f));

    auto* grid = this->scene->addEntity("Grid");
    this->gridID = grid->getUUID();
    auto& gridMesh = grid->addComponent<MeshDynamicComponent>();
    gridMesh.meshBuilder.setMaterial(CHIRA_GET_MATERIAL("MaterialUntextured", "file://materials/unlit.json"));
    for (int i = -5; i <= 5; i++) {
        gridMesh.meshBuilder.addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.025f});
        gridMesh.meshBuilder.addCube(Vertex{{0, 0, i}}, {10.025f, 0.025f, 0.025f});
    }
    gridMesh.meshBuilder.addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
    gridMesh.meshBuilder.addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
    gridMesh.meshBuilder.addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});

    setupKeybinds(cameraTransform);
}

void ModelViewerPanel::addModelSelected() {
    std::string path = FilesystemResourceProvider::getResourceIdentifier(this->modelDialog.GetSelected().string());
    if (!path.empty())
        this->setLoadedFile(path);
}

void ModelViewerPanel::addResourceFolderSelected() {
    std::string resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(this->modelDialog.GetSelected().string());
    if (resourceFolderPath.empty())
        return Device::popupError(TR("error.modelviewer.resource_folder_not_valid"));

    bool resourceExists = false;
    for (const auto& fileProvider : Resource::getResourceProviders(FILESYSTEM_PROVIDER_NAME)) {
        if (resourceFolderPath == assert_cast<FilesystemResourceProvider*>(fileProvider.get())->getFolder()) {
            resourceExists = true;
            break;
        }
    }
    if (!resourceExists)
        Resource::addResourceProvider(new FilesystemResourceProvider{resourceFolderPath});
    else
        Device::popupError(TR("error.modelviewer.resource_folder_already_registered"));
}

void ModelViewerPanel::convertToModelTypeSelected(const std::string& filepath, const std::string& type) const {
    if (!scene->hasEntity(this->previewID))
        return Device::popupError(TR("error.modelviewer.no_model_present"));

    std::ofstream file{filepath, std::ios::binary};
    std::vector<byte> meshData = scene->getEntity(this->previewID)->getComponent<MeshComponent>().getMeshData(type);
    file.write(reinterpret_cast<const char*>(meshData.data()), static_cast<std::streamsize>(meshData.size()));
    file.close();
}

void ModelViewerPanel::convertToOBJSelected() const {
    std::string filepath = this->saveDialogOBJ.GetSelected().string();
    if (filepath.empty())
        return Device::popupError(TR("error.modelviewer.filename_empty"));
    this->convertToModelTypeSelected(filepath, "obj");
}

void ModelViewerPanel::convertToCMDLSelected() const {
    std::string filepath = this->saveDialogCMDL.GetSelected().string();
    if (filepath.empty())
        return Device::popupError(TR("error.modelviewer.filename_empty"));
    this->convertToModelTypeSelected(filepath, "cmdl");
}

void ModelViewerPanel::preRenderContents() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
            if (ImGui::MenuItem(TRC("ui.menubar.open_model"))) // Open Model...
                this->modelDialog.Open();
            if (ImGui::MenuItem(TRC("ui.menubar.add_resource_folder"))) // Add Resource Folder...
                this->folderDialog.Open();
            ImGui::Separator();
            if (ImGui::MenuItem(TRC("ui.menubar.exit"))) // Exit
                Device::queueDestroyWindow(Engine::getMainWindow(), true);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(TRC("ui.menubar.convert"))) { // Convert
            if (ImGui::MenuItem(TRC("ui.menubar.convert_to_obj"))) // Convert to OBJ...
                this->saveDialogOBJ.Open();
            if (ImGui::MenuItem(TRC("ui.menubar.convert_to_cmdl"))) // Convert to CMDL...
                this->saveDialogCMDL.Open();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Model Dialog specific logic
    this->modelDialog.Display();
    if (this->modelDialog.HasSelected()) {
        this->addModelSelected();
        this->modelDialog.ClearSelected();
    }
    this->folderDialog.Display();
    if (this->folderDialog.HasSelected()) {
        this->addResourceFolderSelected();
        this->folderDialog.ClearSelected();
    }
    this->saveDialogOBJ.Display();
    if (this->saveDialogOBJ.HasSelected()) {
        this->convertToOBJSelected();
        this->saveDialogOBJ.ClearSelected();
    }
    this->saveDialogCMDL.Display();
    if (this->saveDialogCMDL.HasSelected()) {
        this->convertToCMDLSelected();
        this->saveDialogCMDL.ClearSelected();
    }

    ImGui::SetNextWindowPos(ImVec2{0, ImGui::GetFrameHeight()});
    ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()});
}

void ModelViewerPanel::renderContents() {
    static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
    static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };

    if (ImGui::BeginChild("evopt_holder", ImVec2(300, ImGui::GetWindowHeight()))) {
        
        if (ImGui::CollapsingHeader("View")) {
            if (this->scene->hasEntity(this->gridID)) {
                ImGui::Checkbox(TRC("ui.editor.show_grid"), &this->showGrid);
                this->scene->getEntity(this->gridID)->setVisible(this->showGrid);
            }
            if (this->scene->hasEntity(this->previewID)) {
                ImGui::Text("%s", this->loadedFile.c_str());
            }
        }

        if (this->selected) {
            if (ImGui::CollapsingHeader("Gizmo")) {
                ImGui::RadioButton("Translate", (int*) &currentGizmoOperation, ImGuizmo::TRANSLATE);
                ImGui::RadioButton("Rotate", (int*) &currentGizmoOperation, ImGuizmo::ROTATE);
                ImGui::RadioButton("Scale", (int*) &currentGizmoOperation, ImGuizmo::SCALE);
                ImGui::Separator();
                ImGui::RadioButton("Local", (int*) &currentGizmoMode, ImGuizmo::LOCAL);
                ImGui::RadioButton("World", (int*) &currentGizmoMode, ImGuizmo::WORLD);
                ImGui::Checkbox("Snap", &useSnap);
                ImGui::InputFloat3("Snap", snap);
            }
        }

        ImGui::EndChild();
    }
    
    if (!this->selected)
        return;

    if (auto* camera = this->scene->getCamera()) {
        const auto view = camera->getView();
        // todo(editor): HACK HACK HACK
        const auto proj = camera->getProjection(Device::getWindowSize(Engine::getMainWindow()));
        glm::mat4 matrix = this->selected->getTransform().getMatrix();

        ImGuizmo::BeginFrame();
        ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
        ImGuizmo::Manipulate(&view[0][0], &proj[0][0], currentGizmoOperation, currentGizmoMode, &matrix[0][0], useSnap ? snap : nullptr);

        this->selected->getTransform().setMatrixLocal(matrix);
    }
}

void ModelViewerPanel::setLoadedFile(const std::string& meshName) {
    if (auto preview = scene->getEntity(this->previewID); preview && meshName == preview->getComponent<MeshComponent>().getMeshResource()->getIdentifier())
        return;
    if (!Resource::hasResource(meshName)) {
        Device::popupError(TRF("error.modelviewer.resource_is_invalid", meshName));
        return;
    }
    if (!scene->hasEntity(this->previewID)) {
        auto* entity = scene->addEntity("Preview");
        this->previewID = entity->getUUID();
    }
    auto* entity = scene->getEntity(this->previewID);
    entity->tryRemoveComponent<MeshComponent>();
    entity->addComponent<MeshComponent>(meshName);
    this->loadedFile = meshName;
}

void ModelViewerPanel::setSelected(Entity* selected_) {
    this->selected = selected_;
}
