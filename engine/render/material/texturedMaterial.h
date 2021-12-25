#pragma once

#include "untexturedMaterial.h"
#include "../texture.h"

namespace chira {
    class texturedMaterial : public untexturedMaterial {
    public:
        explicit texturedMaterial(const std::string& identifier_) : untexturedMaterial(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
    protected:
        sharedPointer<texture> tex;
    private:
        REGISTER_PROPERTY_RESOURCE_TYPE(texturedMaterial);
    };
}
