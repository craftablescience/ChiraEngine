#pragma once

#include <vector>
#include <string>
#include "../utility/vertex.h"

class abstractMeshLoader {
public:
    virtual void loadMesh(const std::string& identifier, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) = 0;
};
