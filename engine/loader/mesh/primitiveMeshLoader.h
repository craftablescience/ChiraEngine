#pragma once

#include "abstractMeshLoader.h"

namespace chira {
    class primitiveMeshLoader : public abstractMeshLoader {
    public:
        void loadMesh(const std::string& identifier, std::vector<vertex>& vertices, std::vector<unsigned int>& indices) override;
    private:
        std::vector<unsigned char> createMesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices) override;
    };
}
