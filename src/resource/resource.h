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
    class resource {
        // To view resource data
        friend class profiler;
    public:
        explicit resource(std::string identifier_) : identifier(std::move(identifier_)) {}
        virtual ~resource();
        virtual void compile(const unsigned char buffer[], std::size_t bufferLength) {}
        [[nodiscard]] const std::string& getIdentifier() const noexcept {
            return this->identifier;
        }
    protected:
        std::string identifier;


    //
    // Static caching functions
    //
    public:
        static void addResourceProvider(const std::string& name, abstractResourceProvider* provider);

        static abstractResourceProvider* getLatestResourceProvider(const std::string& provider);

        static abstractResourceProvider* getResourceProviderWithResource(const std::string& identifier);

        template<typename resourceType, typename... Params>
        static sharedPointer<resourceType> getResource(const std::string& identifier, Params... params) {
            auto id = resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (resource::resources[provider].count(name) > 0) {
                return resource::resources[provider][name].castDynamic<resourceType>();
            }
            return resource::getUniqueResource<resourceType>(identifier, params...);
        }

        template<typename resourceType, typename... Params>
        static void precacheResource(const std::string& identifier, Params... params) {
            auto id = resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (resource::resources[provider].count(name) > 0) {
                return; // Already in cache
            }
            resource::precacheUniqueResource<resourceType>(identifier, params...);
        }

        template<typename resourceType>
        static sharedPointer<resourceType> getCachedResource(const std::string& identifier) {
            auto id = resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (resource::resources[provider].count(name) > 0) {
                return resource::resources[provider][name].castDynamic<resourceType>();
            }
            resource::logResourceError("error.resource.cached_resource_not_found", identifier);
            return sharedPointer<resourceType>{};
        }

        template<typename resourceType, typename... Params>
        static sharedPointer<resourceType> getUniqueResource(const std::string& identifier, Params... params) {
            auto id = resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = resource::providers[provider].rbegin(); i != resource::providers[provider].rend(); i++) {
                if (i->get()->hasResource(name)) {
                    resource::resources[provider][name] = sharedPointer<resource>(new resourceType{identifier, params...});
                    i->get()->compileResource(name, resource::resources[provider][name].get());
                    return resource::resources[provider][name].castDynamic<resourceType>();
                }
            }
            resource::logResourceError("error.resource.resource_not_found", identifier);
            return sharedPointer<resourceType>{};
        }

        template<typename resourceType, typename... Params>
        static void precacheUniqueResource(const std::string& identifier, Params... params) {
            auto id = resource::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = resource::providers[provider].rbegin(); i != resource::providers[provider].rend(); i++) {
                if (i->get()->hasResource(name)) {
                    resource::resources[provider][name] = sharedPointer<resource>(new resourceType{identifier, params...});
                    i->get()->compileResource(name, resource::resources[provider][name].get());
                    return; // Precached!
                }
            }
            resource::logResourceError("error.resource.resource_not_found", identifier);
        }

        /// The only way to make a propertiesResource without a provider is to make it unique, and not to cache it.
        /// You might want to use this sparingly as it defeats the entire point of a cached, shared resource system.
        template<typename resourceType, typename... Params>
        static std::unique_ptr<resourceType> getUniqueUncachedPropertyResource(const std::string& identifier, const nlohmann::json& props, Params... params) {
            auto resource = std::make_unique<resourceType>(identifier, params...);
            resource->compile(props);
            return resource;
        }

        static std::pair<std::string, std::string> splitResourceIdentifier(const std::string& identifier);

        /// If resource is present in the cache and has a reference count less than or equal to 2, mark it for removal.
        static void removeResource(const std::string& identifier);

        /// Delete all resources marked for removal.
        static void cleanup();

        /// Deletes ALL resources and providers. Should only ever be called once, when the program closes.
        static void discardAll();

    protected:
        static std::unordered_map<std::string, std::vector<std::unique_ptr<abstractResourceProvider>>> providers;
        static std::unordered_map<std::string, std::unordered_map<std::string, sharedPointer<resource>>> resources;
        static std::vector<std::string> garbageResources;

        /// We do a few predeclaration workarounds
        static void logResourceError(const std::string& identifier, const std::string& resourceName);
    };
}
