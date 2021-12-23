#pragma once

#include "abstractMeshLoader.h"

namespace chira {
    class chiraMeshLoader : public abstractMeshLoader {
    public:
        void loadMesh(const std::string& identifier, std::vector<vertex>& vertices, std::vector<unsigned int>& indices) override;
        std::vector<unsigned char> createMesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices) override;
    };

    struct chiraMeshHeader {
        unsigned int version = 0;
        unsigned int vertexCount = 0;
        unsigned int indexCount = 0;
    };
    constexpr unsigned short CHIRA_MESH_HEADER_SIZE = sizeof(chiraMeshHeader);
}
