#include "material.h"

#include <i18n/translationManager.h>

using namespace chira;

material::material(const std::string& identifier_) : propertiesResource(identifier_) {}

void material::compile(const nlohmann::json& properties) {
    this->shaderPtr = resource::getResource<shader>(properties["dependencies"]["shader"]);
}

void material::use() {
    this->shaderPtr->use();
}

sharedPointer<shader> material::getShader() {
    return this->shaderPtr;
}
