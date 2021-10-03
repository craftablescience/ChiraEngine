#include "componentManager.h"

void componentManager::addComponent(const std::string& name, abstractComponent* component) {
    this->components[name] = std::unique_ptr<abstractComponent>(component);
}

void componentManager::addEntity(const std::string& name, abstractEntity* entity) {
    this->entities[name] = std::unique_ptr<abstractEntity>(entity);
}

void componentManager::addWorld(const std::string& name, abstractWorld* world) {
    this->worlds[name] = std::unique_ptr<abstractWorld>(world);
}

abstractComponent* componentManager::getComponent(const std::string& name) {
    return this->components[name].get();
}

abstractEntity* componentManager::getEntity(const std::string& name) {
    return this->entities[name].get();
}

abstractWorld* componentManager::getWorld(const std::string& name) {
    return this->worlds[name].get();
}

void componentManager::preRender(double delta) {
    for (const auto& [name, world] : this->worlds) {
        world->preRender(delta);
    }
    for (const auto& [name, entity] : this->entities) {
        entity->preRender(delta);
    }
}

void componentManager::render(double delta) {
    for (const auto& [name, component] : this->components) {
        component->render(delta);
    }
}

void componentManager::postRender(double delta) {
    for (const auto& [name, entity] : this->entities) {
        entity->postRender(delta);
    }
    for (const auto& [name, world] : this->worlds) {
        world->postRender(delta);
    }
}

void componentManager::stop() {
    for (const auto& [name, component] : this->components) {
        component->stop();
    }
    for (const auto& [name, entity] : this->entities) {
        entity->stop();
    }
    for (const auto& [name, world] : this->worlds) {
        world->stop();
    }
}
