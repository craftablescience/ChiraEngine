#include "materialFactory.h"

using namespace chira;

baseMaterial::baseMaterial(const std::string& identifier_) : propertiesResource(identifier_) {}

void baseMaterial::compile(const nlohmann::json& properties) {
    this->shaderPtr = resource::getResource<shader>(properties["dependencies"]["shader"]);
}

void baseMaterial::use() {
    this->shaderPtr->use();
}

sharedPointer<shader> baseMaterial::getShader() {
    return this->shaderPtr;
}

bool materialFactory::registerMaterialType(const std::string& name, materialFactory::factoryFunction createFunc) {
    if (materialFactory::factoryMethods.count(name) > 0)
        return false;
    materialFactory::factoryMethods[name] = std::move(createFunc);
    return true;
}

const materialFactory::factoryFunction& materialFactory::getMaterialType(const std::string& name) {
    return materialFactory::factoryMethods[name];
}
