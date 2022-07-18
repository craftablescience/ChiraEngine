#include "Mesh.h"

#include <math/Matrix.h>

using namespace chira;

void Mesh::update(glm::mat4 parentTransform) {
    if (this->visible)
        this->mesh->render(transformToMatrix(parentTransform, this->position, this->rotation));
    Entity::update(parentTransform);
}
