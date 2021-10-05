#include "abstractEntity.h"

#include "componentManager.h"

using namespace chira;

uuids::uuid abstractEntity::add(abstractComponent* component) {
    auto uuid = componentManager::addComponent(component);
    this->components[uuid] = component;
    return uuid;
}

abstractComponent* abstractEntity::get(const uuids::uuid& uuid) {
    return this->components[uuid];
}

void abstractEntity::remove(const uuids::uuid& uuid) {
    componentManager::removeComponent(uuid);
    this->components.erase(uuid);
}

abstractEntity::~abstractEntity() {
    for (const auto& [uuid, component] : this->components) {
        componentManager::removeComponent(uuid);
    }
    this->components.clear();
}
