#include "entity.h"

#include "../utility/uuidGenerator.h"

using namespace chira;

entity::entity(entity* parent_) {
    this->parent = parent_;
    this->name = uuidGenerator::getNewUUIDString();
}

entity::entity(entity* parent_, const std::string& name_) {
    this->parent = parent_;
    this->name = name_;
}

entity::entity() : entity(nullptr) {}

entity::entity(const std::string& name_) : entity(nullptr, name_) {}

entity::~entity() {
    for (const auto& [name_, ent] : this->children) {
        delete ent;
    }
}

void entity::render(const glm::mat4& parentTransform) {
    for (auto& [key, entity] : this->children) {
        entity->render(parentTransform);
    }
}

entity* entity::getParent() const {
    return this->parent;
}

std::string_view entity::getName() const {
    return this->name;
}

entity* entity::getChild(const std::string& name_) {
    return this->children[name_];
}

bool entity::hasChild(const std::string& name_) {
    return this->children.count(name_) > 0;
}

void entity::addChild(entity* child) {
    child->setParent(this);
    this->children[child->getName().data()] = child;
}

void entity::removeChild(const std::string& name_) {
    delete this->children[name_];
    this->children.erase(name_);
}
