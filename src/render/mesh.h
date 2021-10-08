#pragma once

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include "../loader/abstractMeshLoader.h"
#include "material.h"

namespace chira {
    class mesh : public propertiesResource {
    public:
        mesh(const std::string& identifier_, material* material, glm::vec3* pos = nullptr, glm::quat* rot = nullptr);
        ~mesh() override;
        void compile(const nlohmann::json& properties) override;
        void release() const override;
        void render();
        material* getMaterial() {
            return this->materialPtr;
        }
        void setPosition(glm::vec3* pos) {
            this->position = pos;
        }
        void setRotation(glm::quat* rot) {
            this->rotation = rot;
        }
        static void addMeshLoader(const std::string& name, abstractMeshLoader* meshLoader);
        static abstractMeshLoader* getMeshLoader(const std::string& name);
    protected:
        glm::vec3* position;
        glm::quat* rotation;
    private:
        int depthFunction = GL_LEQUAL;
        bool backfaceCulling = true;
        int cullType = GL_BACK;
        material* materialPtr;
        unsigned int vboHandle = -1, vaoHandle = -1, eboHandle = -1;
        std::vector<vertex> vertices{};
        std::vector<unsigned int> indices{};
        static inline std::unordered_map<std::string, std::unique_ptr<abstractMeshLoader>> meshLoaders{};
        static int getGLDepthFuncFromString(const std::string& depthFunc);
        static int getGLCullTypeFromString(const std::string& cullType);
    };
}
