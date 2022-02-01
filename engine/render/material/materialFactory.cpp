#include "materialFactory.h"

using namespace chira;

MaterialBase::MaterialBase(const std::string& identifier_) : PropertiesResource(identifier_) {}

void MaterialBase::compile(const nlohmann::json& properties) {
    this->shader = Resource::getResource<Shader>(properties["dependencies"]["shader"]);
}

void MaterialBase::use() const {
    this->shader->use();
}

SharedPointer<Shader> MaterialBase::getShader() const {
    return this->shader;
}
