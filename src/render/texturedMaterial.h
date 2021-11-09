#pragma once

#include <utility>
#include "material.h"
#include "texture.h"

namespace chira {
    class texturedMaterial : public material {
    public:
        explicit texturedMaterial(const std::string& identifier_) : material(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        texturedMaterial* copy() override;
        void use() override;
        void release() const override;
    private:
        texture* tex = nullptr;
    };
}
