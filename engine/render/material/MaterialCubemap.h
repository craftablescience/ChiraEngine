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
protected:
    SharedPointer<TextureCubemap> cubemap;
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialCubemap); // NOLINT(cert-err58-cpp)
};

} // namespace chira
