#include "abstractWorld.h"
#include "componentManager.h"

using namespace chira;

uuids::uuid abstractWorld::add(abstractEntity* entity) {
    auto uuid = componentManager::addEntity(entity);
    this->components[uuid] = entity;
    return uuid;
}
