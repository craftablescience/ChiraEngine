#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include <utility/pointer/sharedPointer.h>
#include "provider/abstractResourceProvider.h"
#include "utility/logger.h"

namespace chira {
    constexpr std::string_view RESOURCE_ID_SEPARATOR = "://";

    /// A chunk of data, usually a file. Is typically cached and shared.
    class Resource {
        // To view resource data
        friend class Profiler;
    public:
        explicit Resource(std::string identifier_) : identifier(std::move(identifier_)) {}
        virtual ~Resource();
        virtual void compile(const unsigned char buffer[], std::size_t bufferLength) {}
        [[nodiscard]] const std::string& getIdentifier() const {
            return this->identifier;
        }
    protected:
        std::string identifier;


    //
    // Static caching functions
    //
    public:
        static void addResourceProvider(AbstractResourceProvider* provider);

        static AbstractResourceProvider* getLatestResourceProvider(const std::string& provider);

        static AbstractResourceProvider* getResourceProviderWithResource(const std::string& identifier);

        template<typename ResourceType, typename... Params>
        static SharedPointer<ResourceType> getResource(const std::string& identifier, Params... params) {
            auto id = Resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (Resource::resources[provider].count(name) > 0) {
                return Resource::resources[provider][name].castAssert<ResourceType>();
            }
            return Resource::getUniqueResource<ResourceType>(identifier, params...);
        }

        template<typename ResourceType, typename... Params>
        static void precacheResource(const std::string& identifier, Params... params) {
            auto id = Resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (Resource::resources[provider].count(name) > 0) {
                return; // Already in cache
            }
            Resource::precacheUniqueResource<ResourceType>(identifier, params...);
        }

        template<typename ResourceType>
        static SharedPointer<ResourceType> getCachedResource(const std::string& identifier) {
            auto id = Resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (Resource::resources[provider].count(name) > 0) {
                return Resource::resources[provider][name].castAssert<ResourceType>();
            }
            Resource::logResourceError("error.resource.cached_resource_not_found", identifier);
            return SharedPointer<ResourceType>{};
        }

        template<typename ResourceType, typename... Params>
        static SharedPointer<ResourceType> getUniqueResource(const std::string& identifier, Params... params) {
            auto id = Resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
                if (i->get()->hasResource(name)) {
                    Resource::resources[provider][name] = SharedPointer<Resource>(new ResourceType{identifier, params...});
                    i->get()->compileResource(name, Resource::resources[provider][name].get());
                    return Resource::resources[provider][name].castAssert<ResourceType>();
                }
            }
            Resource::logResourceError("error.resource.resource_not_found", identifier);
            return SharedPointer<ResourceType>{};
        }

        template<typename ResourceType, typename... Params>
        static void precacheUniqueResource(const std::string& identifier, Params... params) {
            auto id = Resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
                if (i->get()->hasResource(name)) {
                    Resource::resources[provider][name] = SharedPointer<Resource>(new ResourceType{identifier, params...});
                    i->get()->compileResource(name, Resource::resources[provider][name].get());
                    return; // Precached!
                }
            }
            Resource::logResourceError("error.resource.resource_not_found", identifier);
        }

        /// The only way to make a propertiesResource without a provider is to make it unique, and not to cache it.
        /// You might want to use this sparingly as it defeats the entire point of a cached, shared resource system.
        template<typename ResourceType, typename... Params>
        static std::unique_ptr<ResourceType> getUniqueUncachedPropertyResource(const std::string& identifier, const nlohmann::json& props, Params... params) {
            auto resource = std::make_unique<ResourceType>(identifier, params...);
            resource->compile(props);
            return resource;
        }

        static std::pair<std::string, std::string> splitResourceIdentifier(const std::string& identifier);

        static const std::vector<std::unique_ptr<AbstractResourceProvider>>& getResourceProviders(const std::string& providerName);

        /// If resource is present in the cache and has a reference count less than or equal to 2, mark it for removal.
        static void removeResource(const std::string& identifier);

        /// Delete all resources marked for removal.
        static void cleanup();

        /// Deletes ALL resources and providers. Should only ever be called once, when the program closes.
        static void discardAll();

    protected:
        static std::unordered_map<std::string, std::vector<std::unique_ptr<AbstractResourceProvider>>> providers;
        static std::unordered_map<std::string, std::unordered_map<std::string, SharedPointer<Resource>>> resources;
        static std::vector<std::string> garbageResources;

        /// We do a few predeclaration workarounds
        static void logResourceError(const std::string& identifier, const std::string& resourceName);
    };
}
