#pragma once

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include "../loader/abstractMeshLoader.h"
#include "abstractMaterial.h"

class mesh {
public:
    explicit mesh(abstractMeshLoader* loader, const std::string& filepath_, const std::string& material_, int depthFunc_ = GL_LEQUAL, bool backfaceCulling_ = true, int cullType_ = GL_BACK);
    virtual ~mesh();
    void setMaterial(const std::string& material_);
    abstractMaterial* getMaterial();
    void compile();
    void discard();
    void render();
    glm::mat4* getModel() {
        return &(this->model);
    };
protected:
    glm::mat4 model;
private:
    int depthFunc;
    bool backfaceCulling;
    int cullType;
    bool compiled = false;
    abstractMeshLoader* loader;
    std::string filepath;
    std::string material;
    unsigned int vboHandle = -1, vaoHandle = -1, eboHandle = -1;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};
