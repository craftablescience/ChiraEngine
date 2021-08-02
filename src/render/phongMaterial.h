#pragma once

#include <utility>

#include "abstractMaterial.h"
#include "../core/engine.h"

class phongMaterial : public abstractMaterial {
public:
    phongMaterial(const std::string& shader_, std::string diffuse_, std::string specular_) : abstractMaterial(shader_), diffuse(std::move(diffuse_)), specular(std::move(specular_)) {
        engine::getTexture(this->diffuse)->setTextureUnit(GL_TEXTURE0);
        engine::getTexture(this->specular)->setTextureUnit(GL_TEXTURE1);
    }
    void compile() override {
        engine::getShader(this->shaderName)->use();
        engine::getShader(this->shaderName)->setUniform("material.diffuse", 0);
        engine::getShader(this->shaderName)->setUniform("material.specular", 1);
    }
    void use() override {
        engine::getTexture(this->diffuse)->use();
        engine::getTexture(this->specular)->use();
        engine::getShader(this->shaderName)->use();
    }
    void setShininess(float shininess = 32.0f) {
        engine::getShader(this->shaderName)->use();
        engine::getShader(this->shaderName)->setUniform("material.shininess", shininess);
    }
    void setLambertFactor(float lambertFactor = 1.0f) {
        engine::getShader(this->shaderName)->use();
        engine::getShader(this->shaderName)->setUniform("material.lambertFactor", lambertFactor);
    }
    void updateLighting(const std::vector<std::unique_ptr<abstractLight>>& lights) override {
        // todo: pass all lighting info to the material (this one)
    }
private:
    std::string diffuse;
    std::string specular;
};
