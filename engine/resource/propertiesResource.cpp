#include "propertiesResource.h"

using namespace chira;

bool propertiesResourceFactory::registerResourceType(const std::string& name, propertiesResourceFactory::factoryFunction createFunc) {
    if (propertiesResourceFactory::factoryMethods.count(name) > 0)
        return false;
    propertiesResourceFactory::factoryMethods[name] = std::move(createFunc);
    return true;
}

const propertiesResourceFactory::factoryFunction& propertiesResourceFactory::getResourceType(const std::string& name) {
    return propertiesResourceFactory::factoryMethods[name];
}
