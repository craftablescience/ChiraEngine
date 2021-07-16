#ifndef BASICGAMEENGINE_OBJMESHLOADER_H
#define BASICGAMEENGINE_OBJMESHLOADER_H


#include <vector>
#include <string>
#include "meshLoader.h"
#include "../utility/vertex.h"

class objMeshLoader : public meshLoader {
public:
    void loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) override;
private:
    static void addVertex(const vertex& v, std::vector<vertex>* vertices, std::vector<unsigned int>* indices);
};


#endif //BASICGAMEENGINE_OBJMESHLOADER_H
