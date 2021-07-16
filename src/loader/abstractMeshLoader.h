#ifndef BASICGAMEENGINE_ABSTRACTMESHLOADER_H
#define BASICGAMEENGINE_ABSTRACTMESHLOADER_H


#include <vector>
#include <string>
#include "../utility/vertex.h"

class abstractMeshLoader {
public:
    virtual void loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) = 0;
};


#endif //BASICGAMEENGINE_ABSTRACTMESHLOADER_H
