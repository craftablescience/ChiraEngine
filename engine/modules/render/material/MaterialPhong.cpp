#include "MaterialPhong.h"

#include <resource/Resource.h>

using namespace chira;

void MaterialPhong::compile(const std::byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    this->shader = Resource::getResource<Shader>(this->shaderPath);
    this->setTextureDiffuse(this->diffusePath);
    this->setTextureSpecular(this->specularPath);
    this->setShininess(this->shininess);
    this->setLambertFactor(this->lambertFactor);
}

void MaterialPhong::use() const {
    IMaterial::use();
    this->diffuse->use(TextureUnit::G0);
    this->specular->use(TextureUnit::G1);
    this->shader->setUniform("material.shininess", this->shininess);
    this->shader->setUniform("material.lambertFactor", this->lambertFactor);
}

SharedPointer<Texture> MaterialPhong::getTextureDiffuse() const {
    return this->diffuse;
}

void MaterialPhong::setTextureDiffuse(std::string path) {
    this->diffusePath = std::move(path);
    this->diffuse = Resource::getResource<Texture>(this->diffusePath);
    this->shader->use();
    this->shader->setUniform("material.diffuse", 0);
}

SharedPointer<Texture> MaterialPhong::getTextureSpecular() const {
    return this->specular;
}

void MaterialPhong::setTextureSpecular(std::string path) {
    this->specularPath = std::move(path);
    this->specular = Resource::getResource<Texture>(this->specularPath);
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
