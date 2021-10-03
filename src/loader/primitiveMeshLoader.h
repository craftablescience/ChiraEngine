#pragma once

#include <vector>
#include "abstractMeshLoader.h"
#include "../utility/vertex.h"

namespace chira {
    class primitiveMeshLoader : public abstractMeshLoader {
    public:
        void loadMesh(const std::string& identifier, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) override;
    };
}
