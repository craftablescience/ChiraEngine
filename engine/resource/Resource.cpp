#include "Resource.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(RESOURCE);

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

void Resource::removeResource(const std::string& identifier) {
    auto id = Resource::splitResourceIdentifier(identifier);
    const std::string& provider = id.first, name = id.second;
    // If the count is 2, then it's being held by the resource manager and the object requesting its removal.
    // Anything below 2 means it should be already deleted everywhere except the resource manager.
    if (Resource::resources[provider].contains(name) && Resource::resources[provider][name].useCount() <= 2)
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
}

void Resource::logResourceError(const std::string& identifier, const std::string& resourceName) {
    LOG_RESOURCE.error(TRF(identifier, resourceName));
}
