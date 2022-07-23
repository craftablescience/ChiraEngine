#include "Entity.h"

#include <algorithm>
#include <i18n/TranslationManager.h>
#include <utility/UUIDGenerator.h>

using namespace chira;

Entity::Entity(std::string name_) : name(std::move(name_)) {}

Entity::Entity() : Entity(UUIDGenerator::getNewUUIDString()) {}

Entity::~Entity() {
    this->removeAllChildren();
}

void Entity::update() { // NOLINT(misc-no-recursion)
    for (auto* entity : this->children) {
        entity->update();
    }
}

void Entity::render(glm::mat4 parentTransform) { // NOLINT(misc-no-recursion)
    glm::mat4 transform = transformToMatrix(parentTransform, this->position, this->rotation);
    for (auto* entity : this->children) {
        if (entity->isVisible()) {
            entity->render(transform);
        }
    }
}

const Frame* Entity::getFrame() const { // NOLINT(misc-no-recursion)
    return this->parent->getFrame();
}

Frame* Entity::getFrame() { // NOLINT(misc-no-recursion)
    return this->parent->getFrame();
}

const Group* Entity::getGroup() const { // NOLINT(misc-no-recursion)
    return this->parent->getGroup();
}

Group* Entity::getGroup() { // NOLINT(misc-no-recursion)
    return this->parent->getGroup();
}

Entity* Entity::getParent() const {
    return this->parent;
}

std::string_view Entity::getName() const {
    return this->name;
}

Entity* Entity::getChild(std::string_view name_) const {
    for (auto* entity : this->children) {
        if (entity->getName() == name_) {
            return entity;
        }
    }
    return nullptr;
}

bool Entity::hasChild(std::string_view name_) const {
    return static_cast<bool>(this->getChild(name_));
}

std::string_view Entity::addChild(Entity* child) {
    // Do not let two children have the same name!
    if (this->hasChild(child->getName())) {
        Logger::log(LogType::LOG_ERROR, "Entity::addChild", TRF("error.entity.duplicate_child_name", child->getName()));
        return child->getName();
    }
    child->setParent(this);
    this->children.push_back(child);
    return child->getName();
}

void Entity::removeChild(std::string_view name_) {
    this->children.erase(std::remove_if(this->children.begin(), this->children.end(), [&name_](Entity* entity) {
        if (entity->getName() == name_) {
            entity->removeAllChildren();
            delete entity;
            return true;
        }
        return false;
    }));
}

void Entity::removeAllChildren() { // NOLINT(misc-no-recursion)
    for (auto* entity : this->children) {
        entity->removeAllChildren();
        delete entity;
    }
    this->children.clear();
}

bool Entity::isVisible() const {
    return this->visible;
}

void Entity::setVisible(bool visibility) {
    this->visible = visibility;
}

void Entity::setPosition(glm::vec3 newPos) {
    this->position = newPos;
}

void Entity::setRotation(glm::quat newRot) {
    this->rotation = newRot;
}

glm::vec3 Entity::getPosition() {
    return this->position;
}

glm::vec3 Entity::getGlobalPosition() { // NOLINT(misc-no-recursion)
    return this->getPosition() + this->parent->getGlobalPosition();
}

glm::quat Entity::getRotation() {
    return this->rotation;
}

void Entity::translate(glm::vec3 translateByAmount) {
    this->position += translateByAmount;
}

void Entity::translateWithRotation(glm::vec3 translateByAmount) {
    glm::quat p{glm::length(translateByAmount), translateByAmount.x, translateByAmount.y, translateByAmount.z};
    p = this->getRotation() * p * glm::conjugate(this->getRotation());
    this->translate(glm::vec3{p.x, p.y, p.z});
}

void Entity::rotate(glm::quat rotateByAmount) {
    this->rotation += rotateByAmount;
}

void Entity::rotate(glm::vec3 rotateByAmount) {
    this->rotation = glm::rotate(this->rotation, rotateByAmount.x, glm::vec3{1,0,0});
    this->rotation = glm::rotate(this->rotation, rotateByAmount.y, glm::vec3{0,1,0});
    this->rotation = glm::rotate(this->rotation, rotateByAmount.z, glm::vec3{0,0,1});
}
