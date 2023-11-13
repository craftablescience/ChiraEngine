#pragma once

#include <glm/glm.hpp>

#include <core/utility/Serial.h>
#include <loader/mesh/IMeshLoader.h>
#include <render/mesh/MeshData.h>

namespace chira {

class MeshDataResource : public Resource, public MeshData {
public:
    explicit MeshDataResource(std::string path_) : Resource(std::move(path_)), MeshData() {}
    void compile(const std::byte buffer[], std::size_t bufferLength) override;

private:
    bool materialSetInCode = false;
    std::string materialType{"MaterialTextured"};
    std::string materialPath{"materials/unlitTextured.json"};
    std::string modelPath{"meshes/missing.cmdl"};
    std::string modelLoader{"cmdl"};

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("materialSetInCode", this->materialSetInCode),
                cereal::make_nvp("materialType", this->materialType),
                cereal::make_nvp("material", this->materialPath),
                cereal::make_nvp("model", this->modelPath),
                cereal::make_nvp("modelLoader", this->modelLoader),
                cereal::make_nvp("depthFunction", this->depthFunction)
        );
    }

private:
    CHIRA_REGISTER_DEFAULT_RESOURCE(MeshDataResource, "meshes/missing.json");
};

} // namespace chira
