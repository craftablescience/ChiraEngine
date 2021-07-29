#pragma once

#include <vector>
#include <string>
#include <functional>
#include "../utility/renderable.h"
#include "../loader/abstractMeshLoader.h"

class mesh : public renderable {
public:
    explicit mesh(abstractMeshLoader* loader, const std::string& filepath_, const std::string& material_, int depthFunc_ = GL_LEQUAL, bool backfaceCulling_ = true, int cullType_ = GL_BACK);
    virtual ~mesh();
    void setMaterial(const std::string& material_);
    std::string getMaterial();
    void compile() override;
    void discard() override;
    void render(shader* shader_) override;
    void render();
private:
    int depthFunc;
    bool backfaceCulling;
    int cullType;
    abstractMeshLoader* loader;
    std::string filepath;
    std::string material;
    unsigned int vboHandle, vaoHandle, eboHandle;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};
