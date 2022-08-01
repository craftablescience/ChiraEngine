#pragma once

#include <glm/glm.hpp>
#include <loader/mesh/IMeshLoader.h>
#include <render/mesh/MeshData.h>

namespace chira {

class MeshDataResource : public PropertiesResource, public MeshData {
public:
    explicit MeshDataResource(std::string identifier_) : PropertiesResource(std::move(identifier_)), MeshData() {}
    void compile(const nlohmann::json& properties) override;
    [[nodiscard]] std::string getDepthFunctionString() const;
    [[nodiscard]] std::string getCullTypeString() const;
    void setDepthFunction(std::string depthFuncStr_);
    void setCullType(std::string cullTypeStr_);
private:
    static int getDepthFunctionFromString(const std::string& depthFunc);
    static int getCullTypeFromString(const std::string& cullType);

    bool materialSetInCode = false;
    std::string materialType{"MaterialTextured"};
    std::string materialPath{"file://materials/unlitTextured.json"};
    std::string modelPath{"file://meshes/missing.cmdl"};
    std::string modelLoader{"cmdl"};
    bool backfaceCulling = true;
    std::string depthFuncStr{"LESS"};
    std::string cullTypeStr{"BACK"};
public:
    CHIRA_PROPS (
            CHIRA_PROP(MeshDataResource, materialSetInCode),
            CHIRA_PROP(MeshDataResource, materialType),
            CHIRA_PROP_NAMED(MeshDataResource, materialPath, material),
            CHIRA_PROP_NAMED(MeshDataResource, modelPath, model),
            CHIRA_PROP_NAMED(MeshDataResource, modelLoader, loader),
            CHIRA_PROP(MeshDataResource, backfaceCulling),
            CHIRA_PROP_NAMED_SET(MeshDataResource, depthFuncStr, depthFunc, setDepthFunction),
            CHIRA_PROP_NAMED_SET(MeshDataResource, cullTypeStr, cullType, setCullType)
    );

private:
    CHIRA_REGISTER_DEFAULT_RESOURCE(MeshDataResource, "file://meshes/missing.json"); // NOLINT(cert-err58-cpp)
};

} // namespace chira
