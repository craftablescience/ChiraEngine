#include "phongMaterial.h"

#include <resource/resource.h>

using namespace chira;

void phongMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->diffuse = resource::getResource<texture>(properties["dependencies"]["diffuse"]);
    this->specular = resource::getResource<texture>(properties["dependencies"]["specular"]);
    this->diffuse->setTextureUnit(GL_TEXTURE0);
    this->specular->setTextureUnit(GL_TEXTURE1);
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.diffuse", 0);
    this->shaderPtr->setUniform("material.specular", 1);
}

void phongMaterial::use() {
    this->diffuse->use();
    this->specular->use();
    material::use();
}

void phongMaterial::setShininess(float shininess) {
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.shininess", shininess);
}

void phongMaterial::setLambertFactor(float lambertFactor) {
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.lambertFactor", lambertFactor);
}
