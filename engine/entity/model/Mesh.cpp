#include "Mesh.h"

#include <math/Matrix.h>

using namespace chira;

void Mesh::render(glm::mat4 parentTransform) {
    if (this->visible)
        this->mesh->render(transformToMatrix(parentTransform, this->position, this->rotation));
    Entity::render(parentTransform);
}
