#pragma once

#include "materialFactory.h"

namespace chira {
    class untexturedMaterial : public baseMaterial {
    public:
        explicit untexturedMaterial(const std::string& identifier_) : baseMaterial(identifier_) {}
    private:
        REGISTER_MATERIAL_TYPE(untexturedMaterial);
    };
}
