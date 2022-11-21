#include "MaterialTextured.h"

// todo(render): move to render backend
#include <glad/gl.h>

using namespace chira;

void MaterialTextured::compile(const nlohmann::json& properties) {
    IMaterial::compile(properties);
    Serialize::fromJSON(this, properties);
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
    this->texture->setTextureUnit(GL_TEXTURE0);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}
