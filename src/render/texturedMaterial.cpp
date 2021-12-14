#include "texturedMaterial.h"

using namespace chira;

void texturedMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->tex = resource::getResource<texture>(properties["dependencies"]["texture"]);
    this->tex->setTextureUnit(GL_TEXTURE0);
    this->shaderPtr->use();
    this->shaderPtr->setUniform("tex", 0);
}

void texturedMaterial::use() {
    this->tex->use();
    material::use();
}
