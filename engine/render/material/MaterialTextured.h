#pragma once

#include "MaterialUntextured.h"
#include "../Texture.h"

namespace chira {

class MaterialTextured : public MaterialUntextured {
public:
    explicit MaterialTextured(std::string identifier_) : MaterialUntextured(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
    [[nodiscard]] SharedPointer<Texture> getTexture() const;
protected:
    SharedPointer<Texture> texture;
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialTextured); // NOLINT(cert-err58-cpp)
};

} // namespace chira
