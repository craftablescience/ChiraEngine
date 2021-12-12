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

const glm::vec3& entity3d::getPosition() {
    return this->position;
}

glm::vec3 entity3d::getGlobalPosition() {
    if (auto parent3d = dynamic_cast<entity3d*>(this->parent)) {
        return this->getPosition() + parent3d->getGlobalPosition();
    }
    return this->getPosition();
}

const glm::quat& entity3d::getRotation() {
    return this->rotation;
}

void entity3d::translate(const glm::vec3& translateByAmount) {
    this->position += translateByAmount;
}

void entity3d::rotate(const glm::quat& rotateByAmount) {
    this->rotation += rotateByAmount;
}
