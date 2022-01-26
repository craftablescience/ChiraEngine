#include "mesh.h"

#include <utility/math/matrix.h>

using namespace chira;

void Mesh::render(glm::mat4 parentTransform) {
    if (this->visible)
        this->mesh->render(transformToMatrix(parentTransform, this->position, this->rotation));
    Entity::render(parentTransform);
}
