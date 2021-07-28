#include "world.h"

world::world(class engine* e, abstractCamera* camera) : meshes{} {
    this->compiled = false;
    this->engine = e;
    this->setCamera(camera);
}

world::~world() {
    delete this->camera;
    if (this->compiled) {
        for (auto &string : this->meshes) {
            engine::getMesh(string)->discard();
        }
    }
}

abstractCamera* world::getCamera() const {
    if (!this->camera) {
        engine::logWarning("world::getCamera", "Must set camera in world::setCamera first");
        return nullptr;
    }
    return this->camera;
}

void world::setCamera(abstractCamera* newCamera) {
    if (this->camera) {
        this->camera->setCurrent(false);
    }
    this->camera = newCamera;
    this->camera->init(this->engine);
    this->camera->setCurrent(true);
}

void world::addMesh(const std::string& mesh) {
    this->meshes.push_back(mesh);
    if (this->compiled) {
        engine::getMesh(mesh)->compile();
    }
}

void world::compile() {
    if (!this->compiled) {
        for (auto& string : this->meshes) {
            engine::getMesh(string)->compile();
        }
        this->compiled = true;
    }
}

void world::discard() {
    if (this->compiled) {
        for (auto& string : this->meshes) {
            engine::getMesh(string)->discard();
        }
        this->compiled = false;
    }
}

void world::render() {
    if (!this->compiled) {
        this->compile();
    }
    for (auto& string : this->meshes) {
        engine::getMesh(string)->render();
    }
}
