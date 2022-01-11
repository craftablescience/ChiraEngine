#include "root.h"

#include <resource/resource.h>

using namespace chira;

Root::Root(const std::string& name_) : World3d(name_) {
    this->skybox = Resource::getResource<MeshResource>("file://meshes/skybox.json");
}

void Root::render() {
    for (auto& [key, entity] : this->children) {
        entity->render(glm::identity<glm::mat4>());
    }
    if (this->renderSkybox) {
        this->skybox->render(glm::identity<glm::mat4>());
    }
}

glm::vec3 Root::getGlobalPosition() {
    return this->position;
}

void Root::setMainCamera(Camera3d* camera) {
    this->mainCamera = camera;
}

Camera3d* Root::getMainCamera() const {
    return this->mainCamera;
}

void Root::setSkybox(const std::string& cubemapId) {
    this->skybox->setMaterial(Resource::getResource<MaterialCubemap>(cubemapId).castAssert<MaterialBase>());
    this->renderSkybox = true;
}

SharedPointer<MaterialCubemap> Root::getSkybox() const {
    return this->skybox->getMaterial().castAssert<MaterialCubemap>();
}

void Root::clearTree() const {
    for (const auto& [name_, ent] : this->children) {
        delete ent;
    }
}
