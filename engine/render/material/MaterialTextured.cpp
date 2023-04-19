#include "MaterialTextured.h"

using namespace chira;

void MaterialTextured::compile(const nlohmann::json& properties) {
    Reflect::fromJSON(this, properties);
}

void MaterialTextured::use() const {
    IMaterial::use();
    this->texture->use();
}

SharedPointer<Texture> MaterialTextured::getTexture() const {
    return this->texture;
}

void MaterialTextured::setTexture(std::string path) {
    this->texturePath = std::move(path);
    this->texture = Resource::getResource<Texture>(this->texturePath);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}
