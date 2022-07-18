#pragma once

#include <string_view>
#include <glm/glm.hpp>
#include <loader/mesh/IMeshLoader.h>
#include <render/mesh/MeshData.h>

namespace chira {

class MeshDataResource : public PropertiesResource, public MeshData {
public:
    explicit MeshDataResource(std::string identifier_) : PropertiesResource(std::move(identifier_)), MeshData() {}
    void compile(const nlohmann::json& properties) override;
private:
    static int getDepthFuncFromString(std::string_view depthFunc);
    static int getCullTypeFromString(std::string_view cullType);

    CHIRA_REGISTER_DEFAULT_RESOURCE(MeshDataResource, "file://meshes/missing.json"); // NOLINT(cert-err58-cpp)
};

}
