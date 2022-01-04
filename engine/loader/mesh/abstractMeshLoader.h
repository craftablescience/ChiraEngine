#pragma once

#include <vector>
#include <string>
#include <utility/math/vertex.h>
#include <utility/primitiveTypes.h>

namespace chira {
    struct AbstractMeshLoader {
        virtual void loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const = 0;
        [[nodiscard]] virtual std::vector<byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) const = 0;
        virtual ~AbstractMeshLoader() = default;
    };
}
