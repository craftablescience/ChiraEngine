#include "root.h"

#include <core/engine.h>
#include <render/ubo.h>
#include "frame.h"

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
    this->mainCamera->createProjection(this->getFrame()->getFrameSize());
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
