#include "componentManager.h"

using namespace chira;

uuids::uuid componentManager::addComponent(abstractComponent* component) {
    uuids::uuid uuid = uuidGenerator::getNewUUID();
    componentManager::components[uuid] = component;
    return uuid;
}

uuids::uuid componentManager::addEntity(abstractEntity* entity) {
    uuids::uuid uuid = uuidGenerator::getNewUUID();
    componentManager::entities[uuid] = entity;
    return uuid;
}

uuids::uuid componentManager::addWorld(abstractWorld* world) {
    uuids::uuid uuid = uuidGenerator::getNewUUID();
    componentManager::worlds[uuid] = world;
    return uuid;
}

void componentManager::removeComponent(const uuids::uuid& uuid) {
    delete componentManager::components[uuid];
    componentManager::components.erase(uuid);
}

void componentManager::removeEntity(const uuids::uuid& uuid) {
    delete componentManager::entities[uuid];
    componentManager::entities.erase(uuid);
}

void componentManager::removeWorld(const uuids::uuid& uuid) {
    delete componentManager::worlds[uuid];
    componentManager::worlds.erase(uuid);
}

void componentManager::render(double delta) {
    for (const auto& [name, world] : componentManager::worlds) {
        world->preRender(delta);
    }
    for (const auto& [name, entity] : componentManager::entities) {
        entity->preRender(delta);
    }

    for (const auto& [name, component] : componentManager::components) {
        component->render(delta);
    }

    for (const auto& [name, entity] : componentManager::entities) {
        entity->postRender(delta);
    }
    for (const auto& [name, world] : componentManager::worlds) {
        world->postRender(delta);
    }
}

void componentManager::clear() {
    componentManager::components.clear();
    componentManager::entities.clear();
    componentManager::worlds.clear();
}
