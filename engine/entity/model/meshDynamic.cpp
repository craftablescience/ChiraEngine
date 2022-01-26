#include "meshDynamic.h"

using namespace chira;

void MeshDynamic::render(glm::mat4 parentTransform) {
    this->mesh.render(transformToMatrix(parentTransform, this->position, this->rotation));
    Entity::render(parentTransform);
}

glm::vec3 MeshDynamic::getAABB() const {
    return {}; // todo: get aabb from mesh
}

MeshDataBuilder* MeshDynamic::getMesh() {
    return &this->mesh;
}
