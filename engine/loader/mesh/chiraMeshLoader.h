#pragma once

#include "abstractMeshLoader.h"

namespace chira {
    class ChiraMeshLoader : public AbstractMeshLoader {
    public:
        void loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) override;
        std::vector<byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override;
    };

    struct ChiraMeshHeader {
        unsigned int version = 0;
        unsigned int vertexCount = 0;
        unsigned int indexCount = 0;
    };
    constexpr unsigned short CHIRA_MESH_HEADER_SIZE = sizeof(ChiraMeshHeader);
}
