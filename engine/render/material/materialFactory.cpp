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

bool MaterialFactory::registerMaterialType(const std::string& name, MaterialFactory::factoryFunction createFunc) {
    if (MaterialFactory::getFactoryMethods().count(name) > 0)
        return false;
    MaterialFactory::getFactoryMethods()[name] = std::move(createFunc);
    return true;
}

const MaterialFactory::factoryFunction& MaterialFactory::getMaterialType(const std::string& name) {
    return MaterialFactory::getFactoryMethods()[name];
}

std::unordered_map<std::string, MaterialFactory::factoryFunction>& MaterialFactory::getFactoryMethods() {
    static std::unordered_map<std::string, factoryFunction> factoryFunctions;
    return factoryFunctions;
}
