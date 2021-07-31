#pragma once

#include "abstractMaterial.h"
#include "../core/engine.h"

class phongMaterial : public abstractMaterial {
public:
    phongMaterial(const std::string& shader, const std::string& diffuse, const std::string& specular) : abstractMaterial(shader) {
        this->diffuse = diffuse;
        this->specular = specular;
        engine::getTexture(this->diffuse)->setTextureUnit(GL_TEXTURE0);
        engine::getTexture(this->specular)->setTextureUnit(GL_TEXTURE1);
    }
    void use() override {
        engine::getTexture(this->diffuse)->use();
        engine::getTexture(this->specular)->use();
        engine::getShader(this->shader)->use();
    }
    void updateLighting(const std::vector<std::unique_ptr<abstractLight>>& lights) override {
        // todo: pass all lighting info to the material (this one)
    }
private:
    std::string diffuse;
    std::string specular;
};