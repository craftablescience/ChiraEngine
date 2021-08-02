#pragma once

#include "abstractMaterial.h"
#include "../core/engine.h"

class unlitMaterial : public abstractMaterial {
public:
    explicit unlitMaterial(const shader& shader_) : abstractMaterial(shader_) {}
    void use() override {
        engine::getShader(this->shaderName)->use();
    }
    void updateLighting(const std::vector<std::unique_ptr<abstractLight>>& lights) override {
        // Unlit lol
    }
};
