#include "entity3d.h"

#include <utility/math/matrix.h>
#include <utility/pointer/assert_cast.h>

using namespace chira;

void Entity3d::render(const glm::mat4& parentTransform) {
    glm::mat4 transform = transformToMatrix(parentTransform, this->position, this->rotation);
    for (auto& [key, entity] : this->children) {
        entity->render(transform);
    }
}

void Entity3d::setPosition(glm::vec3 newPos) {
    this->position = newPos;
}

void Entity3d::setRotation(glm::quat newRot) {
    this->rotation = newRot;
}

glm::vec3 Entity3d::getPosition() {
    return this->position;
}

glm::vec3 Entity3d::getGlobalPosition() { // NOLINT(misc-no-recursion)
    if (auto parent3d = assert_cast<Entity3d*>(this->parent)) {
        return this->getPosition() + parent3d->getGlobalPosition();
    }
    return this->getPosition();
}

glm::quat Entity3d::getRotation() {
    return this->rotation;
}

glm::vec3 Entity3d::getAABB() {
    return {};
}

void Entity3d::translate(glm::vec3 translateByAmount) {
    this->position += translateByAmount;
}

void Entity3d::translateWithRotation(glm::vec3 translateByAmount) {
    glm::quat p{glm::length(translateByAmount), translateByAmount.x, translateByAmount.y, translateByAmount.z};
    p = this->getRotation() * p * glm::conjugate(this->getRotation());
    this->translate(glm::vec3{p.x, p.y, p.z});
}

void Entity3d::rotate(glm::quat rotateByAmount) {
    this->rotation += rotateByAmount;
}

void Entity3d::rotate(glm::vec3 rotateByAmount) {
    this->rotation = glm::rotate(this->rotation, rotateByAmount.x, glm::vec3{1,0,0});
    this->rotation = glm::rotate(this->rotation, rotateByAmount.y, glm::vec3{0,1,0});
    this->rotation = glm::rotate(this->rotation, rotateByAmount.z, glm::vec3{0,0,1});
}
