#include "world.h"

world::world(engine* e, abstractCamera* camera) {
    this->enginePtr = e;
    this->setCamera(camera);
}

world::~world() {
    delete this->camera;
}

abstractCamera* world::getCamera() const {
    if (!this->camera) {
        chira::logger::log(WARN, "world::getCamera", "Must set camera in world::setCamera first");
        return nullptr;
    }
    return this->camera;
}

void world::setCamera(abstractCamera* newCamera) {
    if (this->camera) {
        this->camera->setCurrent(false);
    }
    this->camera = newCamera;
    this->camera->init(this->enginePtr);
    this->camera->setCurrent(true);
}

void world::addMesh(mesh* mesh_) {
    this->meshes.push_back(mesh_);
}

void world::render() {
    for (const auto& meshPtr : this->meshes) {
        // todo: use a UBO or a callback to only set this on each shader once
        meshPtr->getMaterial()->getShader()->setUniform("p", this->camera->getProjectionMatrix());
        meshPtr->getMaterial()->getShader()->setUniform("v", this->camera->getViewMatrix());
        meshPtr->render();
    }
}
