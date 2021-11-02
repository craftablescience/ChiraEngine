#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>
#include "../utility/logger.h"
#include <fmt/core.h>
#include "../i18n/translationManager.h"
#include "provider/abstractResourceProvider.h"

namespace chira {
    constexpr std::string_view RESOURCE_ID_SEPARATOR = "://";

    class resourceManager {
        friend class engine;
    public:
        static void addResourceProvider(const std::string& name, abstractResourceProvider* provider);

        static abstractResourceProvider* getLatestResourceProvider(const std::string& provider);

        static abstractResourceProvider* getResourceProviderWithResource(const std::string& identifier);

        template<typename resourceType, typename... Params>
        static resourceType* getResource(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (const auto& [resourceName, resourcePtr] : resourceManager::resources[provider]) {
                if (name == resourceName) {
                    resourcePtr->incrementRefCount();
                    return dynamic_cast<resourceType*>(resourcePtr);
                }
            }
            return resourceManager::getUniqueResource<resourceType>(identifier, params...);
        }

        template<typename resourceType, typename... Params>
        static void precacheResource(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (const auto& [resourceName, resourcePtr] : resourceManager::resources[provider]) {
                if (name == resourceName) {
                    return; // Already in cache
                }
            }
            resourceManager::precacheUniqueResource<resourceType>(identifier, params...);
        }

        template<typename resourceType>
        static resourceType* getCachedResource(const std::string& identifier) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (const auto& [resourceName, resourcePtr] : resourceManager::resources[provider]) {
                if (name == resourceName) {
                    resourcePtr->incrementRefCount();
                    return dynamic_cast<resourceType*>(resourcePtr);
                }
            }
            logger::log(ERR, "Resource Manager", fmt::format(TR("error.resource_manager.cached_resource_not_found"), identifier));
            return nullptr;
        }

        template<typename resourceType, typename... Params>
        static resourceType* getUniqueResource(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = resourceManager::providers[provider].rbegin(); i != resourceManager::providers[provider].rend(); i++) {
                auto res = i->get()->hasResource(name);
                if (res) {
                    resourceManager::resources[provider][name] = new resourceType{identifier, params...};
                    i->get()->compileResource(name, resourceManager::resources[provider][name]);
                    return dynamic_cast<resourceType*>(resourceManager::resources[provider][name]);
                }
            }
            logger::log(ERR, "Resource Manager", fmt::format(TR("error.resource_manager.resource_not_found"), identifier));
            return nullptr;
        }

        template<typename resourceType, typename... Params>
        static void precacheUniqueResource(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = resourceManager::providers[provider].rbegin(); i != resourceManager::providers[provider].rend(); i++) {
                auto res = i->get()->hasResource(name);
                if (res) {
                    resourceManager::resources[provider][name] = new resourceType{identifier, params...};
                    i->get()->compileResource(name, resourceManager::resources[provider][name]);
                    return; // Precached!
                }
            }
            logger::log(ERR, "Resource Manager", fmt::format(TR("error.resource_manager.resource_not_found"), identifier));
        }

        template<typename resourceType, typename... Params>
        static std::unique_ptr<resourceType> getResourceWithoutCaching(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = resourceManager::providers[provider].rbegin(); i != resourceManager::providers[provider].rend(); i++) {
                auto res = i->get()->hasResource(name);
                if (res) {
                    auto resource = std::make_unique<resourceType>(identifier, params...);
                    i->get()->compileResource(name, resource.get());
                    return resource;
                }
            }
            logger::log(WARN, "Resource Manager", fmt::format(TR("error.resource_manager.resource_not_found"), identifier));
            return nullptr;
        }

        /// The only way to make a propertiesResource without a provider is to make it unique, and not to cache it.
        /// You might want to use this sparingly as it defeats the entire point of a cached, shared resource system.
        template<typename resourceType, typename... Params>
        static std::unique_ptr<resourceType> getUniqueUncachedPropertyResource(const std::string& identifier, const nlohmann::json& props, Params... params) {
            auto resource = std::make_unique<resourceType>(identifier, params...);
            ((propertiesResource*) resource)->compile(props);
            return resource;
        }

        static std::pair<std::string,std::string> splitResourceIdentifier(const std::string& identifier);

        static void removeResource(const std::string& identifier);
    protected:
        /// Deletes ALL resources and providers. Should only ever be called once, when the program closes.
        static void discardAll();
    private:
        static inline std::unordered_map<std::string, std::vector<std::unique_ptr<abstractResourceProvider>>> providers{};
        static inline std::unordered_map<std::string, std::unordered_map<std::string, abstractResource*>> resources{};
    };
}
