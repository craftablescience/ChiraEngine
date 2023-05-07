#pragma once

#include <render/mesh/MeshDataResource.h>
#include "TransformComponent.h"

namespace chira {

struct MeshComponent {
    explicit MeshComponent(const std::string& meshId = "file://meshes/missing.json")
            : transform(nullptr)
            , mesh(Resource::getResource<MeshDataResource>(meshId)) {}

    [[nodiscard]] SharedPointer<MeshDataResource> getMeshResource() const {
        return this->mesh;
    }

    [[nodiscard]] std::vector<byte> getMeshData(const std::string& meshLoader) const {
        return this->mesh->getMeshData(meshLoader);
    }

public:
    TransformComponent* transform;
    SharedPointer<MeshDataResource> mesh;
};

} // namespace chira
