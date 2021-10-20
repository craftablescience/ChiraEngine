#include "skyboxComponent.h"

#include "../resource/resourceManager.h"
#include "../render/cubemapMaterial.h"

using namespace chira;

skyboxComponent::skyboxComponent(const std::string& materialId) {
    this->skyboxMesh = resourceManager::getResource<meshResource>("file://meshes/skybox.json", resourceManager::getResource<cubemapMaterial>(materialId));
}

void skyboxComponent::render(double delta) {
    this->skyboxMesh->render(nullptr, nullptr);
}

skyboxComponent::~skyboxComponent() {
    this->skyboxMesh->release();
}
