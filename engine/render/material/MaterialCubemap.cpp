#include "MaterialCubemap.h"

#include <resource/Resource.h>

using namespace chira;

void MaterialCubemap::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);
}

void MaterialCubemap::use() const {
    IMaterial::use();
    this->cubemap->use();
}

SharedPointer<TextureCubemap> MaterialCubemap::getTextureCubemap() const {
    return this->cubemap;
}

void MaterialCubemap::setTextureCubemap(std::string path) {
    this->cubemapPath = std::move(path);
    this->cubemap = Resource::getResource<TextureCubemap>(this->cubemapPath);
    this->shader->use();
    this->shader->setUniform("cubemap", 0);
}
