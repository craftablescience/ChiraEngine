#pragma once

#include <render/material/MaterialTextured.h>
#include <render/mesh/MeshDataBuilder.h>

namespace chira {

struct MeshSpriteComponent {
    explicit MeshSpriteComponent(glm::vec2 size_ = {1.f, 1.f}, const std::string& materialID = "materials/unlitTextured.json", const std::string& materialType = "MaterialTextured")
            : size(size_) {
        this->sprite.addSquare({}, size, SignedAxis::ZP);
        this->sprite.update();
        this->sprite.setMaterial(CHIRA_GET_MATERIAL(materialType, materialID));
    }

public:
    TransformComponent* transform = nullptr;
    MeshDataBuilder sprite{};
    glm::vec2 size;
};

} // namespace chira
