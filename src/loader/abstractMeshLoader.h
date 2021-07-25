#pragma once

#include <vector>
#include <string>
#include "../utility/vertex.h"

class abstractMeshLoader {
public:
    virtual void loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) = 0;
};
