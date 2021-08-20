#pragma once

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include "../loader/abstractMeshLoader.h"
#include "material.h"

class mesh : public abstractMetaResource {
public:
    explicit mesh(const std::string& provider_, const std::string& name_, const std::shared_ptr<material>& material);
    ~mesh() override;
    void compile(const nlohmann::json& properties) override;
    void render();
    glm::mat4* getModel() {
        return &this->model;
    }
    static void addMeshLoader(const std::string& name, abstractMeshLoader* meshLoader);
    static abstractMeshLoader* getMeshLoader(const std::string& name);
protected:
    glm::mat4 model;
private:
    int depthFunction = GL_LEQUAL;
    bool backfaceCulling = true;
    int cullType = GL_BACK;
    bool compiled = false;
    std::shared_ptr<material> materialPtr;
    unsigned int vboHandle = -1, vaoHandle = -1, eboHandle = -1;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    static inline std::unordered_map<std::string, std::unique_ptr<abstractMeshLoader>> meshLoaders{};
    static int getGLDepthFuncFromString(const std::string& depthFunc);
    static int getGLCullTypeFromString(const std::string& cullType);
};
