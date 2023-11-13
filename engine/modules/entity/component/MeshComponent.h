#pragma once

#include <render/mesh/MeshDataResource.h>
#include "TransformComponent.h"

namespace chira {

struct MeshComponent {
    explicit MeshComponent(const std::string& meshId = "meshes/missing.json")
            : transform(nullptr)
            , mesh(Resource::getResource<MeshDataResource>(meshId)) {}

public:
    TransformComponent* transform;
    SharedPointer<MeshDataResource> mesh;
};

} // namespace chira
