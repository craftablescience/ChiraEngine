#include "material.h"

#include "../resource/resourceManager.h"

material::material(const std::string& provider_, const std::string& name_) : propertiesResource(provider_, name_) {}

void material::compile(const nlohmann::json& properties) {
    this->shaderPtr = resourceManager::getResource<shader>(this->provider, properties["dependencies"]["shader"]);
}

void material::use() {
    this->shaderPtr->use();
}

void material::release() const {
    this->shaderPtr->release();
    abstractResource::release();
}

int material::getGLFormatFromString(const std::string& formatName) {
    if (formatName == "RGB") {
        return GL_RGB;
    } else if (formatName == "RGBA") {
        return GL_RGBA;
    }
    chira::logger::log(WARN, "Material", "Invalid GL format: " + formatName);
    return GL_RGBA;
}

shader* material::getShader() {
    return this->shaderPtr;
}
