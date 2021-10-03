#include "abstractWorld.h"

#include <utility>

const std::string& abstractWorld::getName() {
    return this->name;
}

void abstractWorld::addEntity(const std::string& name_, abstractEntity* component) {
    // todo: add entity to manager
    //this->entities[name_] = std::unique_ptr<abstractEntity>(component);
}

abstractEntity* abstractWorld::getEntity(const std::string& name_) {
    //return this->entities[name_].get();
}

void abstractWorld::removeEntity(const std::string& name_) {
    // todo: remove entity from manager
}

abstractWorld::abstractWorld(std::string name_) : name(std::move(name_)) {}
