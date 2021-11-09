#pragma once

#include "abstractResource.h"
#include <string>
#include <nlohmann/json.hpp>

namespace chira {
    /*
     * Suggested format:
     *
     * {
     *   "dependencies": {
     *     "dep1": "path",
     *     "dep2": "path"
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
    class propertiesResource : public abstractResource {
    public:
        explicit propertiesResource(const std::string& identifier_) : abstractResource(identifier_) {}
        void compile(const unsigned char buffer[], std::size_t bufferLength) final {
            this->compile(nlohmann::json::parse(std::string{reinterpret_cast<const char*>(buffer), bufferLength}));
        }
        virtual void compile(const nlohmann::json& properties) = 0;
        propertiesResource* copy() override {
            this->incrementRefCount();
            return this;
        }

        template<typename T>
        T getPropertyOrDefault(const nlohmann::json& dictionary, const std::string& key, T defaultValue) {
            if (dictionary.contains(key)) {
                return dictionary[key];
            } else {
                return defaultValue;
            }
        }
    };
}
