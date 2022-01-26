#include "root.h"

#include <core/engine.h>

using namespace chira;

Root::Root(Entity* parent_) : Entity(parent_) {
    this->skybox.addCube({}, {1, 1, 1}, false);
    this->skybox.update();
}

Root::Root(Entity* parent_, const std::string& name_) : Entity(parent_, name_) {
    this->skybox.addCube({}, {1, 1, 1}, false);
    this->skybox.update();
}

Root::Root() : Entity() {
    this->skybox.addCube({}, {1, 1, 1}, false);
    this->skybox.update();
}

Root::Root(const std::string& name_) : Entity(name_) {
    this->skybox.addCube({}, {1, 1, 1}, false);
    this->skybox.update();
}

void Root::render() {
    for (auto& [key, entity] : this->children) {
        entity->render(glm::identity<glm::mat4>());
    }
    if (this->renderSkybox) {
        this->skybox.render(glm::identity<glm::mat4>());
    }
}

glm::vec3 Root::getGlobalPosition() {
    return this->position;
}

void Root::setCamera(Camera* camera) {
    this->mainCamera = camera;
    this->mainCamera->createProjection(this->getWindowSize());
}

Camera* Root::getCamera() const {
    return this->mainCamera;
}

void Root::setSkybox(const std::string& cubemapId) {
    this->skybox.setMaterial(Resource::getResource<MaterialCubemap>(cubemapId).castAssert<MaterialBase>());
    this->renderSkybox = true;
}

SharedPointer<MaterialCubemap> Root::getSkybox() const {
    return this->skybox.getMaterial().castAssert<MaterialCubemap>();
}

glm::vec2 Root::getMousePosition() const {
    return Engine::getMousePosition(); //todo(viewport)
}

glm::vec2 Root::getWindowSize() const {
    return Engine::getWindowSize(); //todo(viewport)
}

void Root::clearTree() const {
    for (const auto& [name_, ent] : this->children) {
        delete ent;
    }
}

const Root* Root::getRoot() const {
    return this;
}
