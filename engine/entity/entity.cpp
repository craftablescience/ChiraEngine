#include "entity.h"

#include <utility/uuidGenerator.h>

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
    this->removeAllChildren();
}

void entity::render(const glm::mat4& parentTransform) { // NOLINT(misc-no-recursion)
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

std::string_view entity::addChild(entity* child) {
    child->setParent(this);
    this->children[child->getName().data()] = child;
    return child->getName();
}

void entity::removeChild(const std::string& name_) {
    this->children[name_]->removeAllChildren();
    delete this->children[name_];
    this->children.erase(name_);
}

void entity::removeAllChildren() { // NOLINT(misc-no-recursion)
    for (const auto& [name_, ent_] : this->children) {
        ent_->removeAllChildren();
        delete ent_;
    }
    this->children.clear();
}
