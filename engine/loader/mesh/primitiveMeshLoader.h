#pragma once

#include "abstractMeshLoader.h"

namespace chira {
    class PrimitiveMeshLoader : public AbstractMeshLoader {
    public:
        void loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) override;
    private:
        std::vector<byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override;
    };
}
