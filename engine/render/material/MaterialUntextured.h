#pragma once

#include "MaterialFactory.h"

namespace chira {

class MaterialUntextured : public MaterialBase {
public:
    explicit MaterialUntextured(const std::string& identifier_) : MaterialBase(identifier_) {}
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialUntextured); // NOLINT(cert-err58-cpp)
};

}
