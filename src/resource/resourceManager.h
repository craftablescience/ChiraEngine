#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "../utility/logger.h"
#include "abstractResourceProvider.h"

const std::string RESOURCE_ID_SEPARATOR = "://";

class resourceManager {
    friend class engine;
public:
    static void addResourceProvider(const std::string& name, abstractResourceProvider* provider) {
        if (resourceManager::providers.find(name) == resourceManager::providers.end()) {
            resourceManager::providers[name] = std::vector<std::unique_ptr<abstractResourceProvider>>{};
        }
        resourceManager::providers[name].emplace_back(provider);
    }

    template<typename resourceType, typename... Params>
    static resourceType* getResource(const std::string& provider, const std::string& name, Params... params) {
        for (const auto& [resourceName, resourcePtr] : resourceManager::resources[provider]) {
            if (name == resourceName) {
                return dynamic_cast<resourceType*>(resourcePtr);
            }
        }
        return resourceManager::getUniqueResource<resourceType>(provider, name, params...);
    }

    template<typename resourceType>
    static resourceType* getCachedResource(const std::string& provider, const std::string& name) {
        for (const auto& [resourceName, resourcePtr] : resourceManager::resources[provider]) {
            if (name == resourceName) {
                return dynamic_cast<resourceType*>(resourcePtr);
            }
        }
        chira::logger::log(ERR, "Resource Manager", "Unable to find supposedly cached resource " + provider + RESOURCE_ID_SEPARATOR + name);
        return nullptr;
    }

    template<typename resourceType, typename... Params>
    static resourceType* getUniqueResource(const std::string& provider, const std::string& name, Params... params) {
        for (auto i = resourceManager::providers[provider].rbegin(); i < resourceManager::providers[provider].rend(); i++) {
            auto res = i->get()->hasResource(name);
            if (res) {
                resourceManager::resources[provider][name] = new resourceType{provider, name, params...};
                i->get()->compileResource(name, resourceManager::resources[provider][name]);
                return dynamic_cast<resourceType*>(resourceManager::resources[provider][name]);
            }
        }
        chira::logger::log(ERR, "Resource Manager", "Resource " + provider + RESOURCE_ID_SEPARATOR + name + " was not found");
        return nullptr;
    }

    template<typename resourceType, typename... Params>
    static std::unique_ptr<resourceType> getResourceWithoutCaching(const std::string& provider, const std::string& name, Params... params) {
        for (auto i = resourceManager::providers[provider].rbegin(); i < resourceManager::providers[provider].rend(); i++) {
            auto res = i->get()->hasResource(name);
            if (res) {
                auto resource = std::make_unique<resourceType>(provider, name, params...);
                i->get()->compileResource(name, resource.get());
                return resource;
            }
        }
        chira::logger::log(ERR, "Resource Manager", "Resource " + provider + RESOURCE_ID_SEPARATOR + name + " was not found");
        return nullptr;
    }

    static void removeResource(const std::string& provider, const std::string& name) {
        resourceManager::resources[provider][name]->decrementRefCount();
        if (resourceManager::resources[provider][name]->getRefCount() == 0) {
            delete resourceManager::resources[provider][name];
            resourceManager::resources[provider].erase(name);
        }
    }
protected:
    /*
     * Deletes ALL resources and providers. Should only ever be called once, when the program closes.
     */
    static void discardAll() noexcept {
        for (const auto& [providerName, resourceMap] : resources) {
            for (const auto& [name, resource] : resourceMap) {
                delete resource;
            }
        }
        resourceManager::providers.clear();
    }
private:
    static inline std::unordered_map<std::string, std::vector<std::unique_ptr<abstractResourceProvider>>> providers{};
    static inline std::unordered_map<std::string, std::unordered_map<std::string, abstractResource*>> resources{};
};
