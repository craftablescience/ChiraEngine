#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "../utility/logger.h"
#include "abstractResourceProvider.h"

class resourceManager {
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
        chira::logger::log(ERR, "RM", "Resource " + provider + "\\" + name + " was not found");
        return nullptr;
    }

    static bool removeIfUnused(const std::string& provider, const std::string& name) {
        // use_count will be 2 if one component is holding a pointer, and that will be the component asking for the removal
        if (resourceManager::resources[provider][name]/*.second*/ == 0) {
            resourceManager::resources[provider].erase(name);
            return true;
        } else {
            //resourceManager::resources[provider][name].second--;
            return false;
        }
    }

    // NOTE: Should only ever be called when the program closes
    static void releaseAllResources() noexcept {
        for (const auto& pair : resources) {
            for (const auto& provider : pair.second) {
                // This is bad practice, DO NOT EVER call this function until the very end
                delete provider.second;
            }
        }
        resourceManager::providers.clear();
    }
private:
    static inline std::unordered_map<std::string, std::vector<std::unique_ptr<abstractResourceProvider>>> providers{};
    static inline std::unordered_map<std::string, std::unordered_map<std::string, abstractResource*>> resources{};
};
