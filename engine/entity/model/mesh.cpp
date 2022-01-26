#include "mesh.h"

#include <utility/math/matrix.h>

using namespace chira;

void Mesh::render(glm::mat4 parentTransform) {
    this->mesh->render(transformToMatrix(parentTransform, this->position, this->rotation));
    Entity::render(parentTransform);
}

glm::vec3 Mesh::getAABB() const {
    return {}; // todo: get aabb from mesh
}
