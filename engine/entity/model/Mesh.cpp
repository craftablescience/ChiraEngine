#include "Mesh.h"

#include <math/Matrix.h>

using namespace chira;

Mesh::Mesh(std::string name_, const std::string& meshId) : Entity(std::move(name_)) {
    this->mesh = Resource::getResource<MeshDataResource>(meshId);
}

Mesh::Mesh(const std::string& meshId) : Entity() {
    this->mesh = Resource::getResource<MeshDataResource>(meshId);
}

void Mesh::render(glm::mat4 parentTransform) {
    this->mesh->render(transformToMatrix(parentTransform, this->position, this->rotation));
    Entity::render(parentTransform);
}
