#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include "resource.h"

namespace chira {
    /*
     * Suggested format:
     *
     * {
     *   "dependencies": {
     *     "dep1": "identifier1",
     *     "dep2": "identifier2"
     *   },
     *   "properties": {
     *     "prop1": "value1",
     *     "prop2": "value2"
     *   }
     * }
     *
     * Dependencies and properties are handled by derivative
     * classes, this is only the recommendation on how to
     * store meta-resource files.
     */
    class propertiesResource : public resource {
    public:
        explicit propertiesResource(const std::string& identifier_) : resource(identifier_) {}
        void compile(const unsigned char buffer[], std::size_t bufferLength) final {
            this->compile(nlohmann::json::parse(std::string{reinterpret_cast<const char*>(buffer), bufferLength}));
        }
        virtual void compile(const nlohmann::json& properties) = 0;

        template<typename T>
        T getPropertyOrDefault(const nlohmann::json& dictionary, const std::string& key, T defaultValue) {
            if (dictionary.contains(key))
                return dictionary[key];
            else
                return defaultValue;
        }
    };

    /// Currently used for materials
    class propertiesResourceFactory {
        using factoryFunction = std::function<sharedPointer<propertiesResource>(const std::string&)>;
    public:
        propertiesResourceFactory() = delete;
        static bool registerResourceType(const std::string& name, factoryFunction createFunc);
        static const factoryFunction& getResourceType(const std::string& name);
    private:
        static inline std::unordered_map<std::string, factoryFunction> factoryMethods;
    };
}

#define REGISTER_PROPERTY_RESOURCE_TYPE(resourceClassName)                                     \
    static inline bool resourceClassName##FactoryRegistryHelper =                              \
    chira::propertiesResourceFactory::registerResourceType(                                    \
        #resourceClassName,                                                                    \
        [](const std::string& identifier) -> chira::sharedPointer<chira::propertiesResource> { \
            return chira::resource::getResource<resourceClassName>(identifier)                 \
                   .castReinterpret<chira::propertiesResource>();                              \
        }                                                                                      \
    )

#define GET_PROPERTY(baseType, type, identifier) chira::propertiesResourceFactory::getResourceType(type)(identifier).castReinterpret<baseType>()
