#include "abstractWorld.h"
#include "componentManager.h"

using namespace chira;

uuids::uuid abstractWorld::add(abstractComponent* component) {
    auto uuid = componentManager::addEntity(dynamic_cast<abstractEntity*>(component));
    this->components[uuid] = component;
    return uuid;
}
