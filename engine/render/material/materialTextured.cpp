#include "materialTextured.h"

using namespace chira;

void MaterialTextured::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    this->texture = Resource::getResource<Texture>(properties["dependencies"]["texture"]);
    this->texture->setTextureUnit(GL_TEXTURE0);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}

void MaterialTextured::use() const {
    MaterialUntextured::use();
    this->texture->use();
}

SharedPointer<Texture> MaterialTextured::getTexture() const {
    return this->texture;
}
