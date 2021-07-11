#ifndef BASICGAMEENGINE_MESH_H
#define BASICGAMEENGINE_MESH_H


#include <vector>
#include <string>
#include "../utility/glRenderable.h"
#include "../loader/meshLoader.h"

class mesh : public glRenderable {
public:
    explicit mesh(meshLoader* loader, const std::string& filepath);
    virtual ~mesh();
    void compile() override;
    void discard() override;
    void render() override;
private:
    meshLoader* loader;
    std::string filepath;
    unsigned int vboHandle, vaoHandle, eboHandle;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};


#endif //BASICGAMEENGINE_MESH_H
