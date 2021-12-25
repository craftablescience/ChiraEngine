#include "root.h"

#include <resource/resource.h>

using namespace chira;

root::root(const std::string& name_) : world3d(nullptr, name_) {
    this->skybox = resource::getResource<meshResource>("file://meshes/skybox.json");
}

void root::render() {
    for (auto& [key, entity] : this->children) {
        entity->render(glm::identity<glm::mat4>());
    }
    if (this->renderSkybox) {
        this->skybox->render(glm::identity<glm::mat4>());
    }
}

glm::vec3 root::getGlobalPosition() {
    return this->position;
}

void root::setMainCamera(camera3d* camera) {
    this->mainCamera = camera;
}

camera3d* root::getMainCamera() {
    return this->mainCamera;
}

void root::setSkybox(const std::string& cubemapId) {
    this->skybox->setMaterial(resource::getResource<cubemapMaterial>(cubemapId).castReinterpret<baseMaterial>());
    this->renderSkybox = true;
}

sharedPointer<cubemapMaterial> root::getSkybox() {
    return this->skybox->getMaterial().castReinterpret<cubemapMaterial>();
}

void root::clearTree() {
    for (const auto& [name_, ent] : this->children) {
        delete ent;
    }
}
