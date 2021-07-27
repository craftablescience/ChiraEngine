#pragma once

#include <vector>
#include <string>
#include "../utility/renderable.h"
#include "../loader/abstractMeshLoader.h"

class mesh : public renderable {
public:
    explicit mesh(abstractMeshLoader* loader, const std::string& filepath, int depthFunc_ = GL_LEQUAL, bool backfaceCulling_ = true);
    virtual ~mesh();
    void compile() override;
    void discard() override;
    void render(shader* shader) override;
private:
    int depthFunc;
    bool backfaceCulling;
    abstractMeshLoader* loader;
    std::string filepath;
    unsigned int vboHandle, vaoHandle, eboHandle;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};
