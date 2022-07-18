#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include <event/Events.h>
#include <utility/Logger.h>
#include <utility/SharedPointer.h>
#include <utility/Types.h>
#include "provider/IResourceProvider.h"

namespace chira {

constexpr std::string_view RESOURCE_ID_SEPARATOR = "://";

/// A chunk of data, usually a file. Is typically cached and shared.
class Resource {
    // To view resource data
    friend class Profiler;
public:
    explicit Resource(std::string identifier_) : identifier(std::move(identifier_)) {}
    virtual ~Resource();
    virtual void compile(const unsigned char /*buffer*/[], std::size_t /*bufferLength*/) {}
    [[nodiscard]] std::string_view getIdentifier() const {
        return this->identifier;
    }
protected:
    std::string identifier;

//
// Static caching functions
//
public:
    static void addResourceProvider(IResourceProvider* provider);

    static IResourceProvider* getLatestResourceProvider(const std::string& provider);

    static IResourceProvider* getResourceProviderWithResource(const std::string& identifier);

    template<typename ResourceType, typename... Params>
    static SharedPointer<ResourceType> getResource(const std::string& identifier, Params... params) {
        Resource::cleanup();
        auto id = Resource::splitResourceIdentifier(identifier);
        const std::string& provider = id.first, name = id.second;
        if (Resource::resources[provider].count(name) > 0) {
            return Resource::resources[provider][name].castAssert<ResourceType>();
        }
        return Resource::getUniqueResource<ResourceType>(identifier, std::forward<Params>(params)...);
    }

    template<typename ResourceType, typename... Params>
    static void precacheResource(const std::string& identifier, Params... params) {
        Resource::cleanup();
        auto id = Resource::splitResourceIdentifier(identifier);
        const std::string& provider = id.first, name = id.second;
        if (Resource::resources[provider].count(name) > 0) {
            return; // Already in cache
        }

        for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
            if ((*i)->hasResource(name)) {
                Resource::resources[provider][name] = SharedPointer<Resource>(new ResourceType{identifier, std::forward<Params>(params)...});
                (*i)->compileResource(name, Resource::resources[provider][name].get());
                return; // Precached!
            }
        }
        Resource::logResourceError("error.resource.resource_not_found", identifier);
    }

    template<typename ResourceType>
    static SharedPointer<ResourceType> getCachedResource(const std::string& identifier) {
        Resource::cleanup();
        auto id = Resource::splitResourceIdentifier(identifier);
        const std::string& provider = id.first, name = id.second;
        if (Resource::resources[provider].count(name) > 0) {
            return Resource::resources[provider][name].castAssert<ResourceType>();
        }
        Resource::logResourceError("error.resource.cached_resource_not_found", identifier);
        if (Resource::hasDefaultResource<ResourceType>())
            return Resource::getDefaultResource<ResourceType>();
        return SharedPointer<ResourceType>{};
    }

    template<typename ResourceType, typename... Params>
    static SharedPointer<ResourceType> getUniqueResource(const std::string& identifier, Params... params) {
        auto id = Resource::splitResourceIdentifier(identifier);
        const std::string& provider = id.first, name = id.second;
        for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
            if ((*i)->hasResource(name)) {
                Resource::resources[provider][name] = SharedPointer<Resource>(new ResourceType{identifier, std::forward<Params>(params)...});
                (*i)->compileResource(name, Resource::resources[provider][name].get());
                return Resource::resources[provider][name].castAssert<ResourceType>();
            }
        }
        Resource::logResourceError("error.resource.resource_not_found", identifier);
        if (Resource::hasDefaultResource<ResourceType>())
            return Resource::getDefaultResource<ResourceType>();
        return SharedPointer<ResourceType>{};
    }

    /// You might want to use this sparingly as it defeats the entire point of a cached, shared resource system.
    template<typename ResourceType, typename... Params>
    static std::unique_ptr<ResourceType> getUniqueUncachedResource(const std::string& identifier, Params... params) {
        auto id = Resource::splitResourceIdentifier(identifier);
        const std::string& provider = id.first, name = id.second;
        for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
            if ((*i)->hasResource(name)) {
                auto resource = std::make_unique<ResourceType>(identifier, std::forward<Params>(params)...);
                (*i)->compileResource(name, resource.get());
                return resource;
            }
        }
        Resource::logResourceError("error.resource.resource_not_found", identifier);
        return nullptr;
    }

    /// The only way to make a propertiesResource without a provider is to make it unique, and not to cache it.
    /// You might want to use this sparingly as it defeats the entire point of a cached, shared resource system.
    template<typename ResourceType, typename... Params>
    static std::unique_ptr<ResourceType> getUniqueUncachedPropertyResource(const std::string& identifier, const nlohmann::json& props, Params... params) {
        auto resource = std::make_unique<ResourceType>(identifier, std::forward<Params>(params)...);
        resource->compile(props);
        return resource;
    }

    static std::pair<std::string, std::string> splitResourceIdentifier(const std::string& identifier);

    static const std::vector<std::unique_ptr<IResourceProvider>>& getResourceProviders(const std::string& providerName);

    static bool hasResource(const std::string& identifier);

    /// If resource is present in the cache and has a reference count less than or equal to 2, mark it for removal.
    static void removeResource(const std::string& identifier);

    /// Delete all resources marked for removal.
    static void cleanup();

    /// Deletes ALL resources and providers. Should only ever be called once, when the program closes.
    static void discardAll();

    template<typename ResourceType>
    static uuids::uuid registerDefaultResource(const std::string& identifier) {
        return Events::addListener("chira::engine::create_default_resources", [identifier](const std::any&) {
            Resource::defaultResources[getHashOfType<ResourceType>()] = Resource::getUniqueResource<ResourceType>(identifier).template castAssert<Resource>();
        });
    }

    template<typename ResourceType>
    static bool hasDefaultResource() {
        return Resource::defaultResources.count(getHashOfType<ResourceType>()) > 0;
    }

    template<typename ResourceType>
    static SharedPointer<ResourceType> getDefaultResource() {
        return Resource::defaultResources[getHashOfType<ResourceType>()].template castAssert<ResourceType>();
    }

protected:
    static inline std::unordered_map<std::string, std::vector<std::unique_ptr<IResourceProvider>>> providers;
    static inline std::unordered_map<std::string, std::unordered_map<std::string, SharedPointer<Resource>>> resources;
    static inline std::unordered_map<std::size_t, SharedPointer<Resource>> defaultResources;
    static inline std::vector<std::string> garbageResources;

    /// We do a few predeclaration workarounds
    static void logResourceError(const std::string& identifier, const std::string& resourceName);
};

} // namespace chira

#define CHIRA_REGISTER_DEFAULT_RESOURCE(type, identifier) \
    static inline const uuids::uuid type##DefaultResourceRegistryHelper = \
        chira::Resource::registerDefaultResource<type>(identifier)
