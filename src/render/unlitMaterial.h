#pragma once

#include "abstractMaterial.h"

class unlitMaterial : public abstractMaterial {
    void use() override {
        this->shaderName->use();
    }
    void updateLighting(const std::vector<std::unique_ptr<abstractLight>>& lights) override {
        // Unlit lol
    }
};
