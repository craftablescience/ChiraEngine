#include "cubemapMaterial.h"

#include "../resource/resourceManager.h"

using namespace chira;

void cubemapMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->cubemap = resourceManager::getResource<textureCubemap>(properties["dependencies"]["cubemap"]);
    this->cubemap->setTextureUnit(GL_TEXTURE0);
    this->shaderPtr->use();
    this->shaderPtr->setUniform("cubemap", 0);
}

void cubemapMaterial::use() {
    material::use();
    this->cubemap->use();
}

void cubemapMaterial::release() const {
    material::release();
    this->cubemap->release();
}
