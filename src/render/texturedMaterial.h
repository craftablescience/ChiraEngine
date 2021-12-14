#pragma once

#include "material.h"
#include "texture.h"

namespace chira {
    class texturedMaterial : public material {
    public:
        explicit texturedMaterial(const std::string& identifier_) : material(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
    private:
        sharedPointer<texture> tex;
    };
}
