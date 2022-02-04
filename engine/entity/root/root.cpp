#include "root.h"

#include <core/engine.h>
#include <render/ubo.h>

using namespace chira;

Root::Root(const std::string& name_) : Entity(name_) {
    this->skybox.addCube({}, {1, 1, 1}, false);
    this->skybox.update();
}

Root::Root() : Entity() {
    this->skybox.addCube({}, {1, 1, 1}, false);
    this->skybox.update();
}

void Root::render(glm::mat4 parentTransform) {
    glEnable(GL_DEPTH_TEST); //todo(viewport): remove when a Frame/Window is the new tree root

    if (this->mainCamera)
        UBO_PerspectiveView::get()->update(this->mainCamera->getProjection(), this->mainCamera->getView());

    Entity::render(parentTransform);

    if (this->renderSkybox) {
        // Wiki says modern hardware is fine with this and it looks better
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        this->skybox.render(glm::identity<glm::mat4>());
    }
}

glm::vec3 Root::getGlobalPosition() {
    return this->position;
}

void Root::setCamera(Camera* camera) {
    this->mainCamera = camera;
    this->mainCamera->createProjection(this->getFrameSize());
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

glm::vec2 Root::getMousePosition() const { // NOLINT(misc-no-recursion)
    return Engine::getMousePosition(); //todo(viewport)
    //return this->getRoot()->getMousePosition();
}

glm::vec2 Root::getFrameSize() const { // NOLINT(misc-no-recursion)
    return Engine::getWindowSize(); //todo(viewport)
    //return this->getRoot()->getFrameSize();
}

void Root::clearTree() const {
    for (const auto& [name_, ent] : this->children) {
        delete ent;
    }
}

const Root* Root::getRoot() const {
    return this;
}

Root* Root::getRoot() {
    return this;
}
