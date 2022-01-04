#include "materialTextured.h"

using namespace chira;

void MaterialTextured::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    this->texture = Resource::getResource<Texture>(properties["dependencies"]["texture"]);
    this->texture->setTextureUnit(GL_TEXTURE0);
    this->shader->use();
    this->shader->setUniform("tex", 0); // todo: why is it called this?
}

void MaterialTextured::use() const {
    this->texture->use();
    MaterialUntextured::use();
}

SharedPointer<Texture> MaterialTextured::getTexture() const {
    return this->texture;
}
