#pragma once

#include "Properties.h"
#include "Resource.h"

namespace chira {

class PropertiesResource : public Resource {
public:
    explicit PropertiesResource(std::string identifier_) : Resource(std::move(identifier_)) {}
    void compile(const byte buffer[], std::size_t bufferLength) final;
    virtual void compile(const nlohmann::json& properties) = 0;

    template<typename T>
    [[nodiscard]] static inline T getProperty(const nlohmann::json& properties, const std::string& key, T defaultValue) {
        if (properties.contains(key)) {
            return properties.at(key);
        }
        return defaultValue;
    }
};

} // namespace chira
