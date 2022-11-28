#include "Resource.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(RESOURCE);

Resource::~Resource() {
    Resource::removeResource(this->identifier);
}

//
// Static caching functions
//

void Resource::addResourceProvider(IResourceProvider* provider) {
    std::string providerName{provider->getName().data()};
    if (Resource::providers.find(providerName) == Resource::providers.end()) {
        Resource::providers[providerName] = std::vector<std::unique_ptr<IResourceProvider>>{};
    }
    Resource::providers[providerName].emplace_back(provider);
}

IResourceProvider* Resource::getLatestResourceProvider(const std::string& provider) {
    return Resource::providers[provider][Resource::providers[provider].size() - 1].get();
}

IResourceProvider* Resource::getResourceProviderWithResource(const std::string& identifier) {
    auto id = Resource::splitResourceIdentifier(identifier);
    const std::string& provider = id.first, name = id.second;
    for (auto i = Resource::providers[provider].rbegin(); i < Resource::providers[provider].rend(); i++) {
        if (i->get()->hasResource(name))
            return i->get();
    }
    LOG_RESOURCE.error(TRF("error.resource.resource_not_found", identifier));
    return nullptr;
}

std::pair<std::string, std::string> Resource::splitResourceIdentifier(const std::string& identifier) {
    std::pair<std::string, std::string> out;
    size_t pos = identifier.find(RESOURCE_ID_SEPARATOR);
    if (pos != std::string::npos) {
        out.first = identifier.substr(0, pos);
        out.second = identifier.substr(pos + RESOURCE_ID_SEPARATOR.length());
    } else {
        LOG_RESOURCE.error(TRF("error.resource.cannot_split_identifier", identifier));
    }
    return out;
}

const std::vector<std::unique_ptr<IResourceProvider>>& Resource::getResourceProviders(const std::string& providerName) {
    return Resource::providers.at(providerName);
}

bool Resource::hasResource(const std::string& identifier) {
    auto id = Resource::splitResourceIdentifier(identifier);
    const std::string& provider = id.first, name = id.second;
    for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
        if (i->get()->hasResource(name))
            return true;
    }
    return false;
}

void Resource::removeResource(const std::string& identifier) {
    auto id = Resource::splitResourceIdentifier(identifier);
    const std::string& provider = id.first, name = id.second;
    // If the count is 2, then it's being held by the resource manager and the object requesting its removal.
    // Anything below 2 means it should be already deleted everywhere except the resource manager.
    if (Resource::resources[provider].count(name) > 0 && Resource::resources[provider][name].useCount() <= 2)
        Resource::garbageResources.push_back(identifier);
}

void Resource::cleanup() {
    for (const auto& name : Resource::garbageResources) {
        auto id = Resource::splitResourceIdentifier(name);
        Resource::resources[id.first].erase(id.second);
    }
    Resource::garbageResources.clear();
}

void Resource::discardAll() {
    Resource::defaultResources.clear();
    Resource::cleanup();
    for (const auto& [providerName, resourceMap] : Resource::resources) {
        for (const auto& [name, resource] : resourceMap) {
            // This really shouldn't happen, but it should work out if it does, hence the warning
            LOG_RESOURCE.warning() << TRF("warn.resource.deleting_resource_at_exit",
                                          Resource::resources[providerName].at(name)->getIdentifier(),
                                          Resource::resources[providerName].at(name).useCount());
        }
    }
    Resource::resources.clear();
    Resource::providers.clear();
}

void Resource::logResourceError(const std::string& identifier, const std::string& resourceName) {
    LOG_RESOURCE.error(TRF(identifier, resourceName));
}
