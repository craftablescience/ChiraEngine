#pragma once

#include "MaterialFactory.h"

namespace chira {

class MaterialUntextured : public MaterialBase {
public:
    explicit MaterialUntextured(std::string identifier_) : MaterialBase(std::move(identifier_)) {}
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialUntextured); // NOLINT(cert-err58-cpp)
};

}
