#pragma once

#include "materialFactory.h"

namespace chira {
    class MaterialUntextured : public MaterialBase {
    public:
        explicit MaterialUntextured(const std::string& identifier_) : MaterialBase(identifier_) {}
    private:
        REGISTER_MATERIAL_TYPE(MaterialUntextured);
    };
}
