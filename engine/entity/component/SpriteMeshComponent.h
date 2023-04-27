#pragma once

#include <render/material/MaterialTextured.h>
#include <render/mesh/MeshDataBuilder.h>

namespace chira{

struct SpriteMeshComponent {
    explicit SpriteMeshComponent(const std::string& textureName, glm::vec2 size) {
        this->sprite.addSquare({}, size, SignedAxis::XN);
        this->sprite.update();
        this->sprite.setMaterial(Resource::getResource<MaterialTextured>(textureName).castAssert<IMaterial>());
    }
public:
    MeshDataBuilder sprite{};
}

}