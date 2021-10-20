#pragma once

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include "../loader/abstractMeshLoader.h"
#include "../render/material.h"

namespace chira {
    class meshResource : public propertiesResource {
    public:
        meshResource(const std::string& identifier_, material* material_);
        ~meshResource() override;
        void compile(const nlohmann::json& properties) override;
        void render(glm::vec3* position, glm::quat* rotation);
        void release() const override;
        material* getMaterial() {
            return this->materialPtr;
        }
        static void addMeshLoader(const std::string& name, abstractMeshLoader* meshLoader);
        static abstractMeshLoader* getMeshLoader(const std::string& name);
    private:
        int depthFunction = GL_LEQUAL;
        bool backfaceCulling = true;
        int cullType = GL_BACK;
        material* materialPtr;
        unsigned int vboHandle = -1, vaoHandle = -1, eboHandle = -1;
        std::vector<vertex> vertices{};
        std::vector<unsigned int> indices{};
        static inline std::unordered_map<std::string, std::unique_ptr<abstractMeshLoader>> meshLoaders{};
        static int getDepthFuncFromString(const std::string& depthFunc);
        static int getCullTypeFromString(const std::string& cullType);
    };
}
