#include "materialPhong.h"

#include <resource/resource.h>

using namespace chira;

void MaterialPhong::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    this->diffuse = Resource::getResource<Texture>(properties["dependencies"]["diffuse"]);
    this->specular = Resource::getResource<Texture>(properties["dependencies"]["specular"]);
    this->diffuse->setTextureUnit(GL_TEXTURE0);
    this->specular->setTextureUnit(GL_TEXTURE1);
    this->shader->use();
    this->shader->setUniform("material.diffuse", 0);
    this->shader->setUniform("material.specular", 1);

    this->setShininess(    getPropertyOrDefault(properties["properties"], "shininess",     32.f));
    this->setLambertFactor(getPropertyOrDefault(properties["properties"], "lambertFactor", 1.f));
}

void MaterialPhong::use() const {
    this->diffuse->use();
    this->specular->use();
    MaterialUntextured::use();
}

SharedPointer<Texture> MaterialPhong::getTextureDiffuse() const {
    return this->diffuse;
}

SharedPointer<Texture> MaterialPhong::getTextureSpecular() const {
    return this->specular;
}

void MaterialPhong::setShininess(float shininess) {
    this->shader->use();
    this->shader->setUniform("material.shininess", shininess);
}

void MaterialPhong::setLambertFactor(float lambertFactor) {
    this->shader->use();
    this->shader->setUniform("material.lambertFactor", lambertFactor);
}
