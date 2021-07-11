#ifndef BASICGAMEENGINE_MESHLOADER_H
#define BASICGAMEENGINE_MESHLOADER_H


#include <vector>
#include <string>
#include "../utility/vertex.h"

class meshLoader {
public:
    virtual void loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) = 0;
};


#endif //BASICGAMEENGINE_MESHLOADER_H
