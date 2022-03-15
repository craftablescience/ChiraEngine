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
    class PropertiesResource : public Resource {
    public:
        explicit PropertiesResource(const std::string& identifier_) : Resource(identifier_) {}
        void compile(const unsigned char buffer[], std::size_t bufferLength) final;
        virtual void compile(const nlohmann::json& properties) = 0;

        [[nodiscard]] static bool hasProperty(const nlohmann::json& dictionary, const std::string& key) {
            return dictionary.contains(key);
        }
        template<typename T>
        [[nodiscard]] T getProperty(const nlohmann::json& dictionary, const std::string& key, T defaultValue, bool logError = false) const {
            if (hasProperty(dictionary, key)) {
                return dictionary.at(key);
            } else if (logError) {
                logMissingProperty(this->identifier, key);
            }
            return defaultValue;
        }
    protected:
        static void logMissingProperty(const std::string& identifier, const std::string& key);
    };
}
