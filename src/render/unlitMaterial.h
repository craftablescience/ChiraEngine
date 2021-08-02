#pragma once

#include "abstractMaterial.h"
#include "../core/engine.h"

class unlitMaterial : public abstractMaterial {
public:
    explicit unlitMaterial(const std::string& shader_) : abstractMaterial(shader_) {}
    void compile() override {}
    void use() override {
        engine::getShader(this->shaderName)->use();
    }
    void updateLighting(const std::vector<std::unique_ptr<abstractLight>>& lights) override {
        // Unlit lol
    }
};
