#pragma once

#include <render/mesh/MeshDataBuilder.h>
#include "TransformComponent.h"

namespace chira {

struct MeshDynamicComponent {
public:
    TransformComponent* transform = nullptr;
    MeshDataBuilder meshBuilder{};
};

} // namespace chira
