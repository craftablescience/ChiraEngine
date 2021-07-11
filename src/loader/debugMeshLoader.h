#ifndef BASICGAMEENGINE_DEBUGMESHLOADER_H
#define BASICGAMEENGINE_DEBUGMESHLOADER_H


#include <vector>
#include "meshLoader.h"
#include "../utility/vertex.h"

class debugMeshLoader : public meshLoader {
public:
    void loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) override;
};


#endif //BASICGAMEENGINE_DEBUGMESHLOADER_H
