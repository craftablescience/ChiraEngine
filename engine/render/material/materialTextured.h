#pragma once

#include "materialUntextured.h"
#include "../texture.h"

namespace chira {
    class MaterialTextured : public MaterialUntextured {
    public:
        explicit MaterialTextured(const std::string& identifier_) : MaterialUntextured(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
    protected:
        SharedPointer<Texture> texture;
    private:
        REGISTER_MATERIAL_TYPE(MaterialTextured);
    };
}
