#include "Group.h"

#include <core/Engine.h>
#include <render/UBO.h>
#include "Frame.h"

using namespace chira;

Group::Group(const std::string& name_) : Entity(name_) {}

Group::Group() : Entity() {}

void Group::update(glm::mat4 parentTransform) {
    if (this->mainCamera)
        UBO_PerspectiveView::get()->update(this->mainCamera->getProjection(), this->mainCamera->getView());

    Entity::update(parentTransform);

    if (this->renderSkybox) {
        // Wiki says modern hardware is fine with this and it looks better
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        this->skybox.render(glm::identity<glm::mat4>());
    }

    if (this->mainCamera && Entity::getParent() && Entity::getGroup()->getCamera())
        UBO_PerspectiveView::get()->update(Entity::getGroup()->getCamera()->getProjection(), Entity::getGroup()->getCamera()->getView());
}

glm::vec3 Group::getGlobalPosition() {
    return this->position;
}

void Group::setCamera(Camera* camera) {
    this->mainCamera = camera;
    this->mainCamera->createProjection(this->getFrame()->getFrameSize());
}

Camera* Group::getCamera() const {
    return this->mainCamera;
}

void Group::setSkybox(const std::string& cubemapId) {
    if (!this->skyboxMeshCreated) {
        this->skybox.addCube({}, {1, 1, 1}, false);
        this->skybox.update();
        this->skyboxMeshCreated = true;
    }
    this->skybox.setMaterial(Resource::getResource<MaterialCubemap>(cubemapId).castAssert<MaterialBase>());
    this->renderSkybox = true;
}

SharedPointer<MaterialCubemap> Group::getSkybox() const {
    return this->skybox.getMaterial().castAssert<MaterialCubemap>();
}

const Group* Group::getGroup() const {
    return this;
}

Group* Group::getGroup() {
    return this;
}
