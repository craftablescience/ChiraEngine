#pragma once

#include "MaterialFactory.h"

namespace chira {

class MaterialUntextured final : public IMaterial {
public:
    explicit MaterialUntextured(std::string identifier_) : IMaterial(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
public:
    CHIRA_PROPS_INHERITED(IMaterial) ();
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialUntextured); // NOLINT(cert-err58-cpp)
};

} // namespace chira
