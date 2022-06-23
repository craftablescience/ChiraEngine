#include "MaterialCubemap.h"

#include <resource/Resource.h>

using namespace chira;

void MaterialCubemap::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    this->cubemap = Resource::getResource<TextureCubemap>(getProperty<std::string>(properties["dependencies"], "cubemap", "file://textures/missing.json", true));
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
