#include "entity.h"

#include <utility/uuidGenerator.h>

using namespace chira;

Entity::Entity(Entity* parent_) {
    this->parent = parent_;
    this->name = UUIDGenerator::getNewUUIDString();
}

Entity::Entity(Entity* parent_, const std::string& name_) {
    this->parent = parent_;
    this->name = name_;
}

Entity::Entity() : Entity(nullptr) {}

Entity::Entity(const std::string& name_) : Entity(nullptr, name_) {}

Entity::~Entity() {
    this->removeAllChildren();
}

void Entity::render(const glm::mat4& parentTransform) { // NOLINT(misc-no-recursion)
    for (auto& [key, entity] : this->children) {
        entity->render(parentTransform);
    }
}

Entity* Entity::getParent() const {
    return this->parent;
}

const Root* Entity::getRoot() const { // NOLINT(misc-no-recursion)
    return this->parent->getRoot();
}

std::string_view Entity::getName() const {
    return this->name;
}

bool Entity::hasChild(const std::string& name_) const {
    return this->children.count(name_) > 0;
}

std::string_view Entity::addChild(Entity* child) {
    child->setParent(this);
    this->children[child->getName().data()] = child;
    return child->getName();
}

void Entity::removeChild(const std::string& name_) {
    this->children[name_]->removeAllChildren();
    delete this->children[name_];
    this->children.erase(name_);
}

void Entity::removeAllChildren() { // NOLINT(misc-no-recursion)
    for (const auto& [name_, ent_] : this->children) {
        ent_->removeAllChildren();
        delete ent_;
    }
    this->children.clear();
}
