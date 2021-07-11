#ifndef BASICGAMEENGINE_OBJMESHLOADER_H
#define BASICGAMEENGINE_OBJMESHLOADER_H


#include <vector>
#include "meshLoader.h"
#include "../utility/vertex.h"

class objMeshLoader : public meshLoader {
public:
    void loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) override;
};


#endif //BASICGAMEENGINE_OBJMESHLOADER_H
