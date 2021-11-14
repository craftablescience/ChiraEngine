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
    public:
        static void addResourceProvider(const std::string& name, abstractResourceProvider* provider);

        static abstractResourceProvider* getLatestResourceProvider(const std::string& provider);

        static abstractResourceProvider* getResourceProviderWithResource(const std::string& identifier);

        template<typename resourceType, typename... Params>
        static std::shared_ptr<resourceType> getResource(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (resourceManager::resources[provider].count(name) > 0) {
                return std::dynamic_pointer_cast<resourceType>(resourceManager::resources[provider][name]);
            }
            return resourceManager::getUniqueResource<resourceType>(identifier, params...);
        }

        template<typename resourceType, typename... Params>
        static void precacheResource(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (resourceManager::resources[provider].count(name) > 0) {
                return; // Already in cache
            }
            resourceManager::precacheUniqueResource<resourceType>(identifier, params...);
        }

        template<typename resourceType>
        static std::shared_ptr<resourceType> getCachedResource(const std::string& identifier) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            if (resourceManager::resources[provider].count(name) > 0) {
                return std::dynamic_pointer_cast<resourceType>(resourceManager::resources[provider][name]);
            }
            logger::log(ERR, "Resource Manager", fmt::format(TR("error.resource_manager.cached_resource_not_found"), identifier));
            return nullptr;
        }

        template<typename resourceType, typename... Params>
        static std::shared_ptr<resourceType> getUniqueResource(const std::string& identifier, Params... params) {
            auto id = resourceManager::splitResourceIdentifier(identifier);
            const std::string& provider = id.first, name = id.second;
            for (auto i = resourceManager::providers[provider].rbegin(); i != resourceManager::providers[provider].rend(); i++) {
                if (i->get()->hasResource(name)) {
                    resourceManager::resources[provider][name] = std::make_shared<resourceType>(identifier, params...);
                    i->get()->compileResource(name, resourceManager::resources[provider][name].get());
                    return std::dynamic_pointer_cast<resourceType>(resourceManager::resources[provider][name]);
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
                if (i->get()->hasResource(name)) {
                    resourceManager::resources[provider][name] = std::make_shared<resourceType>(identifier, params...);
                    i->get()->compileResource(name, resourceManager::resources[provider][name].get());
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
                if (i->get()->hasResource(name)) {
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
            resource->compile(props);
            return resource;
        }

        static std::pair<std::string, std::string> splitResourceIdentifier(const std::string& identifier);

        static void removeResource(const std::string& identifier);

        /// Deletes ALL resources and providers. Should only ever be called once, when the program closes.
        static void discardAll();
    private:
        static inline std::unordered_map<std::string, std::vector<std::unique_ptr<abstractResourceProvider>>> providers{};
        static inline std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<resource>>> resources{};
    };
}
