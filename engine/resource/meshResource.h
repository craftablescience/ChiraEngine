#pragma once

#include <utility>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <loader/mesh/abstractMeshLoader.h>
#include "render/material/materialUntextured.h"

namespace chira {
    class MeshResource : public PropertiesResource {
    public:
        explicit MeshResource(const std::string& identifier_) : PropertiesResource(identifier_) {}
        ~MeshResource() override;
        void compile(const nlohmann::json& properties) override;
        void render(const glm::mat4& model);
        [[nodiscard]] SharedPointer<MaterialBase> getMaterial() const {
            return this->material;
        }
        void setMaterial(SharedPointer<MaterialBase> newMaterial) {
            this->material = std::move(newMaterial);
        }

        static void addMeshLoader(const std::string& name, AbstractMeshLoader* meshLoader);
        static AbstractMeshLoader* getMeshLoader(const std::string& name);
    private:
        int depthFunction = GL_LEQUAL;
        bool backfaceCulling = true;
        int cullType = GL_BACK;
        SharedPointer<MaterialBase> material;
        unsigned int vboHandle = -1, vaoHandle = -1, eboHandle = -1;
        std::vector<Vertex> vertices{};
        std::vector<unsigned int> indices{};
        static std::unordered_map<std::string, std::unique_ptr<AbstractMeshLoader>> meshLoaders;
        static int getDepthFuncFromString(const std::string& depthFunc);
        static int getCullTypeFromString(const std::string& cullType);
    };
}
