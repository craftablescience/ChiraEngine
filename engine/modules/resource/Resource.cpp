#include "Resource.h"

#include <core/debug/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(RESOURCE);

Resource::~Resource() {
    Resource::removeResource(this->path);
}

//
// Static caching functions
//

void Resource::removeResource(const std::string& path) {
    // If the count is 2, then it's being held by the resource manager and the object requesting its removal.
    // Anything below 2 means it should be already deleted everywhere except the resource manager.
    if (Resource::resources.contains(path) && Resource::resources[path].useCount() <= 2) {
	    Resource::garbageResources.push_back(path);
    }
}

void Resource::cleanup() {
    for (const auto& path : Resource::garbageResources) {
        Resource::resources.erase(path);
    }
    Resource::garbageResources.clear();
}

void Resource::discardAll() {
    Resource::defaultResources.clear();
    Resource::cleanup();
    for (const auto& [path, resource] : Resource::resources) {
		// This shouldn't happen, but it should still exit cleanly if it does
		LOG_RESOURCE.warning() << TRF("warn.resource.deleting_resource_at_exit",
									  Resource::resources.at(path)->getPath(),
									  Resource::resources.at(path).useCount());
    }
    Resource::resources.clear();
}

void Resource::logResourceError(const std::string& path, const std::string& resourceName) {
    LOG_RESOURCE.error(TRF(path, resourceName));
}
