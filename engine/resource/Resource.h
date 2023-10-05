#pragma once

#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/Logger.h>
#include <math/Types.h>
#include <utility/SharedPointer.h>
#include <utility/Types.h>

namespace chira {

constexpr std::string_view RESOURCE_ID_SEPARATOR = "://";

/// A chunk of data, usually a file. Is typically cached and shared.
class Resource {
    // To view resource data
    friend class ResourceUsageTrackerPanel;
public:
    explicit Resource(std::string identifier_)
            : identifier(std::move(identifier_)) {}

    virtual ~Resource() {
        Resource::removeResource(this->identifier);
    }

    virtual void compile(const byte /*buffer*/[], std::size_t /*bufferLength*/) = 0;

    [[nodiscard]] std::string_view getIdentifier() const {
        return this->identifier;
    }

protected:
    std::string identifier;

//
// Static caching functions
//
public:
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

        std::filesystem::path resourcePath;
        resourcePath = std::filesystem::current_path().append("assets/engine").append(name);
        std::uintmax_t fileSize = std::filesystem::file_size(resourcePath);
        std::ifstream ifs(resourcePath.string().c_str(), std::ios::in | std::ios::binary);
        ifs.seekg(0, std::ios::beg);
        auto* bytes = new byte[(std::size_t) fileSize + 1];
        ifs.read(reinterpret_cast<char*>(bytes), static_cast<std::streamsize>(fileSize));
        bytes[fileSize] = '\0';
        Resource::resources[provider][name] = SharedPointer<Resource>(new ResourceType{identifier, std::forward<Params>(params)...});
        Resource::resources[provider][name]->compile(bytes, (std::size_t) fileSize + 1);
        delete[] bytes;

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

//        for (auto i = Resource::providers[provider].rbegin(); i != Resource::providers[provider].rend(); i++) {
//            if ((*i)->hasResource(name)) {
//                Resource::resources[provider][name] = SharedPointer<Resource>(new ResourceType{identifier, std::forward<Params>(params)...});
//                (*i)->compileResource(name, Resource::resources[provider][name].get());
//                return Resource::resources[provider][name].template cast<ResourceType>();
//            }
//        }

        std::filesystem::path resourcePath;
        resourcePath = std::filesystem::current_path().append("assets/engine").append(name);
        std::uintmax_t fileSize = std::filesystem::file_size(resourcePath);
        std::ifstream ifs(resourcePath.string().c_str(), std::ios::in | std::ios::binary);
        ifs.seekg(0, std::ios::beg);
        auto* bytes = new byte[(std::size_t) fileSize + 1];
        ifs.read(reinterpret_cast<char*>(bytes), static_cast<std::streamsize>(fileSize));
        bytes[fileSize] = '\0';
        Resource::resources[provider][name] = SharedPointer<Resource>(new ResourceType{identifier, std::forward<Params>(params)...});
        Resource::resources[provider][name]->compile(bytes, (std::size_t) fileSize + 1);
        delete[] bytes;
        return Resource::resources[provider][name].template cast<ResourceType>();
    }

    static std::pair<std::string, std::string> splitResourceIdentifier(const std::string& identifier);

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
