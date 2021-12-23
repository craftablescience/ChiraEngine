#include "entity3d.h"

#include <utility/math/matrix.h>

using namespace chira;

void entity3d::render(const glm::mat4& parentTransform) {
    glm::mat4 transform = transformToMatrix(parentTransform, this->position, this->rotation);
    for (auto& [key, entity] : this->children) {
        entity->render(transform);
    }
}

void entity3d::setPosition(const glm::vec3& newPos) {
    this->position = newPos;
}

void entity3d::setRotation(const glm::quat& newRot) {
    this->rotation = newRot;
}

glm::vec3 entity3d::getPosition() {
    return this->position;
}

glm::vec3 entity3d::getGlobalPosition() { // NOLINT(misc-no-recursion)
    if (auto parent3d = dynamic_cast<entity3d*>(this->parent)) {
        return this->getPosition() + parent3d->getGlobalPosition();
    }
    return this->getPosition();
}

glm::quat entity3d::getRotation() {
    return this->rotation;
}

void entity3d::translate(const glm::vec3& translateByAmount) {
    this->position += translateByAmount;
}

void entity3d::translateWithRotation(const glm::vec3& translateByAmount) {
    glm::quat p{glm::length(translateByAmount), translateByAmount.x, translateByAmount.y, translateByAmount.z};
    p = this->getRotation() * p * glm::conjugate(this->getRotation());
    this->translate(glm::vec3{p.x, p.y, p.z});
}

void entity3d::rotate(const glm::quat& rotateByAmount) {
    this->rotation += rotateByAmount;
}

void entity3d::rotate(const glm::vec3& rotateByAmount) {
    this->rotation = glm::rotate(this->rotation, rotateByAmount.x, glm::vec3{1,0,0});
    this->rotation = glm::rotate(this->rotation, rotateByAmount.y, glm::vec3{0,1,0});
    this->rotation = glm::rotate(this->rotation, rotateByAmount.z, glm::vec3{0,0,1});
}
