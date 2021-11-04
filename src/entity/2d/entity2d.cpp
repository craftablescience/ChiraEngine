#include "entity2d.h"

#include "../../utility/math/matrix.h"

using namespace chira;

void entity2d::render(const glm::mat4& parentTransform) {
    glm::mat4 transform = transformToMatrix(parentTransform, this->position, this->rotation);
    for (auto& [key, entity] : this->children) {
        entity->render(transform);
    }
}

void entity2d::setPosition(const glm::vec2& newPos) {
    this->position = newPos;
}

void entity2d::setRotation(const glm::quat& newRot) {
    this->rotation = newRot;
}

const glm::vec2& entity2d::getPosition() {
    return this->position;
}

const glm::quat& entity2d::getRotation() {
    return this->rotation;
}

void entity2d::translate(const glm::vec2& translateByAmount) {
    this->position += translateByAmount;
}

void entity2d::rotate(const glm::quat& rotateByAmount) {
    this->rotation += rotateByAmount;
}
