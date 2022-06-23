#include "IMeshLoader.h"

using namespace chira;

void IMeshLoader::addMeshLoader(const std::string& name, IMeshLoader* meshLoader) {
    IMeshLoader::meshLoaders[name] = std::unique_ptr<IMeshLoader>(meshLoader);
}

IMeshLoader* IMeshLoader::getMeshLoader(const std::string& name) {
    return IMeshLoader::meshLoaders[name].get();
}
