#include "MaterialFactory.h"

using namespace chira;

MaterialBase::MaterialBase(std::string identifier_) : PropertiesResource(std::move(identifier_)) {}

void MaterialBase::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);
}

void MaterialBase::use() const {
    this->shader->use();
}

SharedPointer<Shader> MaterialBase::getShader() const {
    return this->shader;
}

void MaterialBase::setShader(std::string path) {
    this->shaderPath = std::move(path);
    this->shader = Resource::getResource<Shader>(this->shaderPath);
}
