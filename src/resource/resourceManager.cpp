#include "resourceManager.h"

void resourceManager::addResourceProvider(const std::string& name, abstractResourceProvider* provider) {
    if (resourceManager::providers.find(name) == resourceManager::providers.end()) {
        resourceManager::providers[name] = std::vector<std::unique_ptr<abstractResourceProvider>>{};
    }
    resourceManager::providers[name].emplace_back(provider);
}

abstractResourceProvider* resourceManager::getLatestResourceProvider(const std::string& name) {
    return resourceManager::providers[name][resourceManager::providers[name].size() - 1].get();
}

void resourceManager::removeResource(const std::string& provider, const std::string& name) {
    resourceManager::resources[provider][name]->decrementRefCount();
    if (resourceManager::resources[provider][name]->getRefCount() == 0) {
        delete resourceManager::resources[provider][name];
        resourceManager::resources[provider].erase(name);
    }
}

void resourceManager::discardAll() {
    for (const auto& [providerName, resourceMap] : resources) {
        for (const auto& [name, resource] : resourceMap) {
            delete resource;
        }
    }
    resourceManager::providers.clear();
}
