#pragma once

#include "Properties.h"
#include "Resource.h"

namespace chira {

class PropertiesResource : public Resource {
public:
    explicit PropertiesResource(std::string identifier_) : Resource(std::move(identifier_)) {}
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
    static void logMissingProperty(std::string_view identifier, std::string_view key);
};

} // namespace chira
