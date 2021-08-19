#pragma once

#include <vector>
#include <string>
#include "../utility/vertex.h"

class abstractMeshLoader {
public:
    virtual void loadMesh(const std::string& provider, const std::string& name, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) = 0;
};
