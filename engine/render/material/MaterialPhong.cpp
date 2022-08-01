#include "MaterialPhong.h"

#include <resource/Resource.h>

using namespace chira;

void MaterialPhong::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    Serialize::fromJSON(this, properties);
}

void MaterialPhong::use() const {
    MaterialUntextured::use();
    this->diffuse->use();
    this->specular->use();
}

SharedPointer<Texture> MaterialPhong::getTextureDiffuse() const {
    return this->diffuse;
}

void MaterialPhong::setTextureDiffuse(std::string path) {
    this->diffusePath = std::move(path);
    this->diffuse = Resource::getResource<Texture>(this->diffusePath);
    this->diffuse->setTextureUnit(GL_TEXTURE0);
    this->shader->use();
    this->shader->setUniform("material.diffuse", 0);
}

SharedPointer<Texture> MaterialPhong::getTextureSpecular() const {
    return this->specular;
}

void MaterialPhong::setTextureSpecular(std::string path) {
    this->specularPath = std::move(path);
    this->specular = Resource::getResource<Texture>(this->specularPath);
    this->specular->setTextureUnit(GL_TEXTURE1);
    this->shader->use();
    this->shader->setUniform("material.specular", 1);
}

float MaterialPhong::getShininess() const {
    return this->shininess;
}

void MaterialPhong::setShininess(float shininess_) {
    this->shininess = shininess_;
    this->shader->use();
    this->shader->setUniform("material.shininess", this->shininess);
}

float MaterialPhong::getLambertFactor() const {
    return this->lambertFactor;
}

void MaterialPhong::setLambertFactor(float lambertFactor_) {
    this->lambertFactor = lambertFactor_;
    this->shader->use();
    this->shader->setUniform("material.lambertFactor", this->lambertFactor);
}
