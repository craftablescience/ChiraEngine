#pragma once

#include <render/mesh/MeshDataResource.h>
#include "TransformComponent.h"

namespace chira {

struct MeshComponent {
    explicit MeshComponent(const std::string& meshId)
            : mesh(Resource::getResource<MeshDataResource>(meshId))
            , transform(nullptr) {}

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
