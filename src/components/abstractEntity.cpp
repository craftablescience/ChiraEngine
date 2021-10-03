#include "abstractEntity.h"

void abstractEntity::addComponent(const std::string& name, abstractComponent* component) {
    this->components[name] = component;
}

abstractComponent* abstractEntity::getComponent(const std::string& name) {
    return this->components[name];
}
