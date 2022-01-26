#include "abstractMeshLoader.h"

using namespace chira;

void AbstractMeshLoader::addMeshLoader(const std::string& name, AbstractMeshLoader* meshLoader) {
    AbstractMeshLoader::meshLoaders[name] = std::unique_ptr<AbstractMeshLoader>(meshLoader);
}

AbstractMeshLoader* AbstractMeshLoader::getMeshLoader(const std::string& name) {
    return AbstractMeshLoader::meshLoaders[name].get();
}
