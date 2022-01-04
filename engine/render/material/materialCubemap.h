#pragma once

#include "materialUntextured.h"
#include "../textureCubemap.h"

namespace chira {
    class MaterialCubemap : public MaterialUntextured {
    public:
        explicit MaterialCubemap(const std::string& identifier_) : MaterialUntextured(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
    protected:
        SharedPointer<TextureCubemap> cubemap;
    private:
        REGISTER_MATERIAL_TYPE(MaterialCubemap);
    };
}
