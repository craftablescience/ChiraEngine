#include "materialFactory.h"

using namespace chira;

MaterialBase::MaterialBase(const std::string& identifier_) : PropertiesResource(identifier_) {}

void MaterialBase::compile(const nlohmann::json& properties) {
    this->shader = Resource::getResource<Shader>(properties["dependencies"]["shader"]);
}

void MaterialBase::use() {
    this->shader->use();
}

SharedPointer<Shader> MaterialBase::getShader() const {
    return this->shader;
}

bool MaterialFactory::registerMaterialType(const std::string& name, MaterialFactory::factoryFunction createFunc) {
    if (MaterialFactory::factoryMethods.count(name) > 0)
        return false;
    MaterialFactory::factoryMethods[name] = std::move(createFunc);
    return true;
}

const MaterialFactory::factoryFunction& MaterialFactory::getMaterialType(const std::string& name) {
    return MaterialFactory::factoryMethods[name];
}
