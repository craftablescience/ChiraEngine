#include "phongMaterial.h"

#include "../resource/resourceManager.h"

void phongMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->diffuse = resourceManager::getResource<texture2d>(
            this->provider,
            properties["dependencies"]["diffuse"],
            material::getGLFormatFromString(properties["properties"]["diffuse_format"]));
    this->specular = resourceManager::getResource<texture2d>(
            this->provider,
            properties["dependencies"]["specular"],
            material::getGLFormatFromString(properties["properties"]["specular_format"]));
    this->diffuse->setTextureUnit(GL_TEXTURE0);
    this->specular->setTextureUnit(GL_TEXTURE1);
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.diffuse", 0);
    this->shaderPtr->setUniform("material.specular", 1);
}

void phongMaterial::use() {
    this->diffuse->use();
    this->specular->use();
    this->shaderPtr->use();
}

void phongMaterial::setShininess(float shininess) {
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.shininess", shininess);
}

void phongMaterial::setLambertFactor(float lambertFactor) {
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.lambertFactor", lambertFactor);
}
