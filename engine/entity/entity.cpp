#include "entity.h"

#include <algorithm>
#include <i18n/translationManager.h>
#include <utility/logger.h>
#include <utility/uuidGenerator.h>

using namespace chira;

Entity::Entity(std::string name_) : name(std::move(name_)) {}

Entity::Entity() : Entity(UUIDGenerator::getNewUUIDString()) {}

Entity::~Entity() {
    this->removeAllChildren();
}

void Entity::render(glm::mat4 parentTransform) { // NOLINT(misc-no-recursion)
    glm::mat4 transform = transformToMatrix(parentTransform, this->position, this->rotation);
    for (const auto& key : this->childrenOrder)
        this->children[key]->render(transform);
}

const Window* Entity::getWindow() const { // NOLINT(misc-no-recursion)
    return this->parent->getWindow();
}

Window* Entity::getWindow() { // NOLINT(misc-no-recursion)
    return this->parent->getWindow();
}

const Frame* Entity::getFrame() const { // NOLINT(misc-no-recursion)
    return this->parent->getFrame();
}

Frame* Entity::getFrame() { // NOLINT(misc-no-recursion)
    return this->parent->getFrame();
}

const Root* Entity::getRoot() const { // NOLINT(misc-no-recursion)
    return this->parent->getRoot();
}

Root* Entity::getRoot() { // NOLINT(misc-no-recursion)
    return this->parent->getRoot();
}

Entity* Entity::getParent() const {
    return this->parent;
}

std::string Entity::getName() const {
    return this->name;
}

Entity* Entity::getChild(const std::string& name_) const {
    return this->children.at(name_);
}

bool Entity::hasChild(const std::string& name_) const {
    return static_cast<bool>(this->children.count(name_));
}

std::string Entity::addChild(Entity* child) {
    // Do not let two children have the same name!
    if (this->hasChild(child->getName())) {
        Logger::log(LogType::ERROR, "Entity::addChild", TRF("error.entity.duplicate_child_name", child->getName()));
        return child->getName();
    }
    child->setParent(this);
    this->children[child->getName()] = child;
    this->childrenOrder.push_back(child->getName());
    return child->getName();
}

void Entity::removeChild(const std::string& name_) {
    this->children[name_]->removeAllChildren();
    delete this->children[name_];
    this->children.erase(name_);
    std::ignore = std::remove(this->childrenOrder.begin(), this->childrenOrder.end(), name_);
}

void Entity::removeAllChildren() { // NOLINT(misc-no-recursion)
    for (const auto& [name_, ent_] : this->children) {
        ent_->removeAllChildren();
        delete ent_;
    }
    this->children.clear();
    this->childrenOrder.clear();
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
