#pragma once

#include <render/texture/TextureCubemap.h>
#include "MaterialUntextured.h"

namespace chira {

class MaterialCubemap final : public IMaterial {
public:
    explicit MaterialCubemap(std::string identifier_) : IMaterial(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
    [[nodiscard]] SharedPointer<TextureCubemap> getTextureCubemap() const;
    void setTextureCubemap(std::string path);
protected:
    SharedPointer<TextureCubemap> cubemap;
    std::string cubemapPath{"file://textures/missing.json"};
public:
    CHIRA_PROPS_INHERITED(IMaterial) (
            CHIRA_PROP_NAMED_SET(MaterialCubemap, cubemapPath, cubemap, setTextureCubemap)
    );
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialCubemap);
};

} // namespace chira
