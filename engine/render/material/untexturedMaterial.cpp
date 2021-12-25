#include "untexturedMaterial.h"

#include <i18n/translationManager.h>

using namespace chira;

untexturedMaterial::untexturedMaterial(const std::string& identifier_) : propertiesResource(identifier_) {}

void untexturedMaterial::compile(const nlohmann::json& properties) {
    this->shaderPtr = resource::getResource<shader>(properties["dependencies"]["shader"]);
}

void untexturedMaterial::use() {
    this->shaderPtr->use();
}

sharedPointer<shader> untexturedMaterial::getShader() {
    return this->shaderPtr;
}
