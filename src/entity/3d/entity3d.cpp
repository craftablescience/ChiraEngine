#include "entity3d.h"

#include "../../utility/math/matrix.h"

using namespace chira;

void entity3d::render(const glm::mat4& parentTransform) {
    for (auto& [key, entity] : this->children) {
        entity->render(transformToMatrix(parentTransform, this->position, this->rotation));
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

const glm::quat& entity3d::getRotation() {
    return this->rotation;
}

void entity3d::translate(const glm::vec3& translateByAmount) {
    this->position += translateByAmount;
}

void entity3d::rotate(const glm::quat& rotateByAmount) {
    this->rotation += rotateByAmount;
}
