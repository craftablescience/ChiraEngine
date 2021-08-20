#include "world.h"
#include "../resource/resourceManager.h"

world::world(engine* e, abstractCamera* camera) {
    this->enginePtr = e;
    this->setCamera(camera);
}

world::~world() {
    delete this->camera;
}

abstractCamera* world::getCamera() const {
    if (!this->camera) {
        chiraLogger::log(WARN, "world::getCamera", "Must set camera in world::setCamera first");
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

void world::addMesh(const std::shared_ptr<mesh>& mesh_) {
    this->meshes.push_back(mesh_);
}

void world::render() {
    for (const auto& meshPtr : this->meshes) {
        meshPtr->render();
    }
}
