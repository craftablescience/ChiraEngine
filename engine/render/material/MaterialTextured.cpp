#include "MaterialTextured.h"

using namespace chira;

void MaterialTextured::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    Serialize::fromJSON(this, properties);
}

void MaterialTextured::use() const {
    MaterialUntextured::use();
    this->texture->use();
}

SharedPointer<Texture> MaterialTextured::getTexture() const {
    return this->texture;
}

void MaterialTextured::setTexture(std::string path) {
    this->texturePath = std::move(path);
    this->texture = Resource::getResource<Texture>(this->texturePath);
    this->texture->setTextureUnit(GL_TEXTURE0);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}
