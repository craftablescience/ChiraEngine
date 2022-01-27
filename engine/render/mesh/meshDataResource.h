#pragma once

#include <glm/glm.hpp>
#include <loader/mesh/abstractMeshLoader.h>
#include <render/mesh/meshData.h>

namespace chira {
    class MeshDataResource : public PropertiesResource, public MeshData {
    public:
        explicit MeshDataResource(const std::string& identifier_) : PropertiesResource(identifier_), MeshData() {}
        void compile(const nlohmann::json& properties) override;
    private:
        static int getDepthFuncFromString(const std::string& depthFunc);
        static int getCullTypeFromString(const std::string& cullType);
    };
}