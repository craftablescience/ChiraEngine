#include "materialCubemap.h"

#include <resource/resource.h>

using namespace chira;

void MaterialCubemap::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    this->cubemap = Resource::getResource<TextureCubemap>(properties["dependencies"]["cubemap"]);
    this->cubemap->setTextureUnit(GL_TEXTURE0);
    this->shader->use();
    this->shader->setUniform("cubemap", 0);
}

void MaterialCubemap::use() const {
    MaterialUntextured::use();
    this->cubemap->use();
}

SharedPointer<TextureCubemap> MaterialCubemap::getTextureCubemap() const {
    return this->cubemap;
}
