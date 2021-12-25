#include "cubemapMaterial.h"

#include <resource/resource.h>

using namespace chira;

void cubemapMaterial::compile(const nlohmann::json& properties) {
    untexturedMaterial::compile(properties);
    this->cubemap = resource::getResource<textureCubemap>(properties["dependencies"]["cubemap"]);
    this->cubemap->setTextureUnit(GL_TEXTURE0);
    this->shaderPtr->use();
    this->shaderPtr->setUniform("cubemap", 0);
}

void cubemapMaterial::use() {
    untexturedMaterial::use();
    this->cubemap->use();
}
