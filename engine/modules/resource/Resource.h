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

namespace chira {

constexpr std::string_view RESOURCE_ID_SEPARATOR = "://";

/// A chunk of data, usually a file. Is typically cached and shared.
class Resource {
    // To view resource data
    friend class ResourceUsageTrackerPanel;
public:
    explicit Resource(std::string identifier_)
            : identifier(std::move(identifier_)) {}

    virtual ~Resource();

    virtual void compile(const std::byte /*buffer*/[], std::size_t /*bufferLength*/) = 0;

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
            return Resource::resources[provider][name].template cast<ResourceType>();
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
            return Resource::resources[provider][name].cast<ResourceType>();
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
                return Resource::resources[provider][name].template cast<ResourceType>();
            }
        }
        Resource::logResourceError("error.resource.resource_not_found", identifier);
        if (Resource::hasDefaultResource<ResourceType>())
            return Resource::getDefaultResource<ResourceType>();
        return SharedPointer<ResourceType>{};
    }

    /// You might want to use this sparingly as it defeats the entire point of a cached, shared resource system.
    template<typename ResourceType, typename... Params>
    static SharedPointer<ResourceType> getUniqueUncachedResource(const std::string& identifier, Params... params) {
        auto id = Resource::splitResourceIdentifier(identifier);
        const std::string& provider = id.first, name = id.second;
        for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
            if ((*i)->hasResource(name)) {
                auto resource = SharedPointer<ResourceType>(new ResourceType{identifier, std::forward<Params>(params)...});
                (*i)->compileResource(name, resource.get());
                // We're not holding onto this
                resource.setHolderAmountForDelete(0);
                return resource;
            }
        }
        Resource::logResourceError("error.resource.resource_not_found", identifier);
        return SharedPointer<ResourceType>{};
    }

    /// The only way to make a PropertiesResource without a provider is to make it unique, and not to cache it.
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
    static bool registerDefaultResource(const std::string& identifier) {
        Resource::getDefaultResourceConstructors()[typeHash<ResourceType>()] = [identifier] {
            Resource::defaultResources[typeHash<ResourceType>()] = Resource::getUniqueResource<ResourceType>(identifier).template cast<Resource>();
        };
        return true;
    }

    template<typename ResourceType>
    static bool hasDefaultResource() {
        return Resource::defaultResources.contains(typeHash<ResourceType>());
    }

    template<typename ResourceType>
    static SharedPointer<ResourceType> getDefaultResource() {
        return Resource::defaultResources[typeHash<ResourceType>()].template cast<ResourceType>();
    }

    static void createDefaultResources() {
        for (const auto& [id, ctor] : Resource::getDefaultResourceConstructors()) {
            ctor();
        }
    }

protected:
    static inline std::unordered_map<std::string, std::vector<std::unique_ptr<IResourceProvider>>> providers;
    static inline std::unordered_map<std::string, std::unordered_map<std::string, SharedPointer<Resource>>> resources;
    static inline std::unordered_map<std::type_index, SharedPointer<Resource>> defaultResources;
    static inline std::vector<std::string> garbageResources;

    static auto getDefaultResourceConstructors() -> std::unordered_map<std::type_index, std::function<void()>>& {
        static std::unordered_map<std::type_index, std::function<void()>> defaultResourceConstructors;
        return defaultResourceConstructors;
    }

    /// We do a few predeclaration workarounds
    static void logResourceError(const std::string& identifier, const std::string& resourceName);
};

} // namespace chira

#define CHIRA_REGISTER_DEFAULT_RESOURCE(type, identifier) \
    static inline const auto type##DefaultResourceRegistryHelper = \
        chira::Resource::registerDefaultResource<type>(identifier)
