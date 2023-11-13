#pragma once

#include "Resource.h"

namespace chira {

class JSONResource : public Resource {
public:
    explicit JSONResource(std::string path_) : Resource(std::move(path_)) {}
    void compile(const std::byte buffer[], std::size_t bufferLength) final;
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
