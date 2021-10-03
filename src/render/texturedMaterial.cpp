#include "texturedMaterial.h"

#include "../resource/resourceManager.h"

using namespace chira;

void texturedMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->tex = resourceManager::getResource<texture2d>(
            properties["dependencies"]["texture"],
            material::getGLFormatFromString(properties["properties"]["texture_format"]));
    this->tex->setTextureUnit(GL_TEXTURE0);
    this->shaderPtr->use();
    this->shaderPtr->setUniform("tex", 0);
}

void texturedMaterial::use() {
    this->tex->use();
    material::use();
}

void texturedMaterial::release() const {
    this->tex->release();
    material::release();
}
