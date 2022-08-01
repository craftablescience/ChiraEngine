#pragma once

#include "MaterialUntextured.h"
#include "../TextureCubemap.h"

namespace chira {

class MaterialCubemap : public MaterialUntextured {
public:
    explicit MaterialCubemap(std::string identifier_) : MaterialUntextured(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
    [[nodiscard]] SharedPointer<TextureCubemap> getTextureCubemap() const;
    void setTextureCubemap(std::string path);
protected:
    SharedPointer<TextureCubemap> cubemap;
    std::string cubemapPath{"file://textures/missing.json"};
public:
    CHIRA_PROPS (
            CHIRA_PROP_NAMED_SET(MaterialCubemap, cubemapPath, cubemap, setTextureCubemap)
    );
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialCubemap); // NOLINT(cert-err58-cpp)
};

} // namespace chira
