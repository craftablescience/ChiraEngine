#pragma once

#include <vector>
#include <string>
#include "../utility/math/vertex.h"

namespace chira {
    class abstractMeshLoader {
    public:
        virtual void loadMesh(const std::string& identifier, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) = 0;
    };
}
