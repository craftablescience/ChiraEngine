#include "material.h"

#include <fmt/core.h>
#include "../i18n/translationManager.h"
#include "../resource/resourceManager.h"

using namespace chira;

material::material(const std::string& identifier_) : propertiesResource(identifier_) {}

void material::compile(const nlohmann::json& properties) {
    this->shaderPtr = resourceManager::getResource<shader>(properties["dependencies"]["shader"]);
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
    logger::log(WARN, "Material", fmt::format(TR("warn.material.invalid_gl_format"), formatName));
    return GL_RGBA;
}

shader* material::getShader() {
    return this->shaderPtr;
}
