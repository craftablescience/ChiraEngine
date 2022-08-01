#include "MaterialCubemap.h"

#include <resource/Resource.h>

using namespace chira;

void MaterialCubemap::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    Serialize::fromJSON(this, properties);
}

void MaterialCubemap::use() const {
    MaterialUntextured::use();
    this->cubemap->use();
}

SharedPointer<TextureCubemap> MaterialCubemap::getTextureCubemap() const {
    return this->cubemap;
}

void MaterialCubemap::setTextureCubemap(std::string path) {
    this->cubemapPath = std::move(path);
    this->cubemap = Resource::getResource<TextureCubemap>(this->cubemapPath);
    this->cubemap->setTextureUnit(GL_TEXTURE0);
    this->shader->use();
    this->shader->setUniform("cubemap", 0);
}
