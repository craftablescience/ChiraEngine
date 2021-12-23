#include "resource.h"

#include <i18n/translationManager.h>
#include <utility/logger.h>

using namespace chira;

resource::~resource() {
    resource::removeResource(this->identifier);
}


//
// Static caching functions
//

std::unordered_map<std::string, std::vector<std::unique_ptr<abstractResourceProvider>>> resource::providers{};
std::unordered_map<std::string, std::unordered_map<std::string, sharedPointer<resource>>> resource::resources{};
std::vector<std::string> resource::garbageResources{};

void resource::addResourceProvider(abstractResourceProvider* provider) {
    if (resource::providers.find(provider->getName()) == resource::providers.end()) {
        resource::providers[provider->getName()] = std::vector<std::unique_ptr<abstractResourceProvider>>{};
    }
    resource::providers[provider->getName()].emplace_back(provider);
}

abstractResourceProvider* resource::getLatestResourceProvider(const std::string& provider) {
    return resource::providers[provider][resource::providers[provider].size() - 1].get();
}

abstractResourceProvider* resource::getResourceProviderWithResource(const std::string& identifier) {
    auto id = resource::splitResourceIdentifier(identifier);
    const std::string& provider = id.first, name = id.second;
    for (auto i = resource::providers[provider].rbegin(); i < resource::providers[provider].rend(); i++) {
        if (i->get()->hasResource(name)) {
            return i->get();
        }
    }
    logger::log(ERR, "Resource", fmt::format(TR("error.resource.resource_not_found"), identifier));
    return nullptr;
}

std::pair<std::string, std::string> resource::splitResourceIdentifier(const std::string& identifier) {
    std::pair<std::string, std::string> out;
    size_t pos = identifier.find(RESOURCE_ID_SEPARATOR);
    if (pos != std::string::npos) {
        out.first = identifier.substr(0, pos);
        out.second = identifier.substr(pos + RESOURCE_ID_SEPARATOR.length());
    } else {
        logger::log(ERR, "Resource", fmt::format(TR("error.resource.cannot_split_identifier"), identifier));
    }
    return out;
}

void resource::removeResource(const std::string& identifier) {
    auto id = resource::splitResourceIdentifier(identifier);
    const std::string& provider = id.first, name = id.second;
    // If the count is 2, then it's being held by the resource manager and the object requesting its removal.
    // Anything below 2 means it should be already deleted everywhere except the resource manager.
    if (resource::resources[provider].count(name) > 0 && resource::resources[provider][name].useCount() <= 2) {
        resource::garbageResources.push_back(identifier);
    }
}

void resource::cleanup() {
    for (const auto& name : resource::garbageResources) {
        auto id = resource::splitResourceIdentifier(name);
        resource::resources[id.first].erase(id.second);
    }
    resource::garbageResources.clear();
}

void resource::discardAll() {
    resource::cleanup();
    for (const auto& [providerName, resourceMap] : resource::resources) {
        for (const auto& [name, resource] : resourceMap) {
            // This really shouldn't happen, but it should work out if it does, hence the warning
            logger::log(WARN, "Resource", fmt::format(TR("warn.resource.deleting_resource_at_exit"), resource::resources[providerName].at(name)->getIdentifier(), resource::resources[providerName].at(name).useCount()));
        }
    }
    resource::resources.clear();
    resource::providers.clear();
}

void resource::logResourceError(const std::string& identifier, const std::string& resourceName) {
    logger::log(ERR, "Resource", fmt::format(TR(identifier), resourceName));
}
