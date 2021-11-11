#include "phongMaterial.h"

#include "../resource/resourceManager.h"

using namespace chira;

void phongMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->diffuse = resourceManager::getResource<texture>(properties["dependencies"]["diffuse"]);
    this->specular = resourceManager::getResource<texture>(properties["dependencies"]["specular"]);
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

phongMaterial::~phongMaterial() {
    resourceManager::removeResource(this->diffuse->getIdentifier());
    resourceManager::removeResource(this->specular->getIdentifier());
}

void phongMaterial::setShininess(float shininess) {
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.shininess", shininess);
}

void phongMaterial::setLambertFactor(float lambertFactor) {
    this->shaderPtr->use();
    this->shaderPtr->setUniform("material.lambertFactor", lambertFactor);
}
