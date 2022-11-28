#pragma once

#include "MaterialFactory.h"

namespace chira {

class MaterialUntextured final : public IMaterial {
public:
    explicit MaterialUntextured(std::string identifier_) : IMaterial(std::move(identifier_)) {}
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialUntextured); // NOLINT(cert-err58-cpp)
};

} // namespace chira
