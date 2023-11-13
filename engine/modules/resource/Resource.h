#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/debug/Logger.h>
#include <core/math/Types.h>
#include <core/utility/SharedPointer.h>
#include <core/utility/Types.h>
#include <core/FileSystem.h>

namespace chira {

/// A chunk of data, usually a file. Is typically cached and shared.
class Resource {
    // To view resource data
    friend class ResourceUsageTrackerPanel;

public:
    explicit Resource(std::string path_)
            : path(std::move(path_)) {}

    virtual ~Resource();

    virtual void compile(const std::byte /*buffer*/[], std::size_t /*bufferLength*/) = 0;

    [[nodiscard]] std::string_view getPath() const {
        return this->path;
    }

protected:
    std::string path;

	// Static caching functions
public:
    template<typename ResourceType, typename... Params>
    static SharedPointer<ResourceType> getResource(const std::string& path, Params... params) {
        Resource::cleanup();

        if (Resource::resources.contains(path)) {
            return Resource::resources[path].template cast<ResourceType>();
        }
        return Resource::getUniqueResource<ResourceType>(path, std::forward<Params>(params)...);
    }

    template<typename ResourceType, typename... Params>
    static void precacheResource(const std::string& path, Params... params) {
        Resource::cleanup();

        if (Resource::resources.contains(path)) {
            return; // Already in cache
        }

        if (!FileSystem::exists(path)) {
	        Resource::logResourceError("error.resource.resource_not_found", path);
			return;
		}

		std::vector<std::byte> data;
		if (!FileSystem::readBytes(path, data)) {
			Resource::logResourceError("error.resource.resource_not_found", path);
			return;
		}
		data.push_back(static_cast<std::byte>(0));

		Resource::resources[path] = SharedPointer<Resource>(new ResourceType{path, std::forward<Params>(params)...});
		Resource::resources[path]->compile(data.data(), data.size());
    }

    template<typename ResourceType>
    static SharedPointer<ResourceType> getCachedResource(const std::string& path) {
        Resource::cleanup();

        if (Resource::resources.contains(path)) {
            return Resource::resources[path].cast<ResourceType>();
        }

        Resource::logResourceError("error.resource.cached_resource_not_found", path);
        if (Resource::hasDefaultResource<ResourceType>()) {
	        return Resource::getDefaultResource<ResourceType>();
        }
        return SharedPointer<ResourceType>{};
    }

    template<typename ResourceType, typename... Params>
    static SharedPointer<ResourceType> getUniqueResource(const std::string& path, Params... params) {
	    if (!FileSystem::exists(path)) {
		    Resource::logResourceError("error.resource.resource_not_found", path);
		    if (Resource::hasDefaultResource<ResourceType>()) {
			    return Resource::getDefaultResource<ResourceType>();
		    }
		    return SharedPointer<ResourceType>{};
	    }

		std::vector<std::byte> data;
		if (!FileSystem::readBytes(path, data)) {
			Resource::logResourceError("error.resource.resource_not_found", path);
			if (Resource::hasDefaultResource<ResourceType>()) {
				return Resource::getDefaultResource<ResourceType>();
			}
			return SharedPointer<ResourceType>{};
		}
		data.push_back(static_cast<std::byte>(0));

		Resource::resources[path] = SharedPointer<Resource>(new ResourceType{path, std::forward<Params>(params)...});
		Resource::resources[path]->compile(data.data(), data.size());
		return Resource::resources[path].template cast<ResourceType>();
    }

    /// You might want to use this sparingly as it defeats the entire point of a cached, shared resource system.
    template<typename ResourceType, typename... Params>
    static SharedPointer<ResourceType> getUniqueUncachedResource(const std::string& path, Params... params) {
        if (!FileSystem::exists(path)) {
	        Resource::logResourceError("error.resource.resource_not_found", path);
	        return SharedPointer<ResourceType>{};
		}

		std::vector<std::byte> data;
		if (!FileSystem::readBytes(path, data)) {
			Resource::logResourceError("error.resource.resource_not_found", path);
			return SharedPointer<ResourceType>{};
		}
		data.push_back(static_cast<std::byte>(0));

		auto resource = SharedPointer<ResourceType>(new ResourceType{path, std::forward<Params>(params)...});
		resource->compile(data.data(), data.size());
		// We're not holding onto this
		resource.setHolderAmountForDelete(0);
		return resource;
    }

    /// If resource is present in the cache and has a reference count less than or equal to 2, mark it for removal.
    static void removeResource(const std::string& path);

    /// Delete all resources marked for removal.
    static void cleanup();

    /// Deletes ALL resources and providers. Should only ever be called once, when the program closes.
    static void discardAll();

    template<typename ResourceType>
    static bool registerDefaultResource(const std::string& path) {
        Resource::getDefaultResourceConstructors()[typeIndex<ResourceType>()] = [path] {
            Resource::defaultResources[typeIndex<ResourceType>()] = Resource::getUniqueResource<ResourceType>(path).template cast<Resource>();
        };
        return true;
    }

    template<typename ResourceType>
    static bool hasDefaultResource() {
        return Resource::defaultResources.contains(typeIndex<ResourceType>());
    }

    template<typename ResourceType>
    static SharedPointer<ResourceType> getDefaultResource() {
        return Resource::defaultResources[typeIndex<ResourceType>()].template cast<ResourceType>();
    }

    static void createDefaultResources() {
        for (const auto& [id, ctor] : Resource::getDefaultResourceConstructors()) {
            ctor();
        }
    }

protected:
    static inline std::unordered_map<std::string, SharedPointer<Resource>> resources;
    static inline std::unordered_map<std::type_index, SharedPointer<Resource>> defaultResources;
    static inline std::vector<std::string> garbageResources;

    static auto getDefaultResourceConstructors() -> std::unordered_map<std::type_index, std::function<void()>>& {
        static std::unordered_map<std::type_index, std::function<void()>> defaultResourceConstructors;
        return defaultResourceConstructors;
    }

    /// We do a few predeclaration workarounds
    static void logResourceError(const std::string& path, const std::string& resourceName);
};

} // namespace chira

#define CHIRA_REGISTER_DEFAULT_RESOURCE(type, path) \
    static inline const auto type##DefaultResourceRegistryHelper = \
        chira::Resource::registerDefaultResource<type>(path)
