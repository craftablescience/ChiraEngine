#include "world.h"

world::world(engine* e, abstractCamera* camera) {
    this->enginePtr = e;
    this->setCamera(camera);
}

world::~world() {
    delete this->camera;
    if (this->compiled) {
        for (const auto& string : this->meshes) {
            engine::getMesh(string)->discard();
        }
    }
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

void world::addMesh(const std::string& mesh) {
    this->meshes.push_back(mesh);
    if (this->compiled) {
        engine::getMesh(mesh)->compile();
    }
}

unsigned int world::addLight(abstractLight* light) {
    this->lights.emplace_back(light);
    this->lightsDirty = true;
    return this->lights.size() - 1;
}

abstractLight* world::getLight(unsigned int lightId) {
    return this->lights.at(lightId).get();
}

void world::compile() {
    if (!this->compiled) {
        for (const auto& string : this->meshes) {
            engine::getMesh(string)->compile();
        }
        this->compiled = true;
    }
}

void world::discard() {
    if (this->compiled) {
        for (const auto& string : this->meshes) {
            engine::getMesh(string)->discard();
        }
        this->compiled = false;
    }
}

void world::render() {
    if (!this->compiled) {
        this->compile();
    }
    for (const auto& string : this->meshes) {
        if (this->lightsDirty) {
            engine::getMesh(string)->getMaterial()->updateLighting(this->lights);
            this->lightsDirty = false;
        }
        engine::getMesh(string)->render();
    }
}
