#include "MaterialFactory.h"

using namespace chira;

IMaterial::IMaterial(std::string identifier_) : PropertiesResource(std::move(identifier_)) {}

void IMaterial::compile(const nlohmann::json& properties) {
    Reflect::fromJSON(this, properties);
}

void IMaterial::use() const {
    this->shader->use();
}

SharedPointer<Shader> IMaterial::getShader() const {
    return this->shader;
}

void IMaterial::setShader(std::string path) {
    this->shaderPath = std::move(path);
    this->shader = Resource::getResource<Shader>(this->shaderPath);
}
