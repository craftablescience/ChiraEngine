#pragma once

#include <string>
#include <string_view>

namespace chira {

class Resource;

class IResourceProvider {
public:
    explicit IResourceProvider(std::string name) : providerName(std::move(name)) {}
    virtual ~IResourceProvider() = default;
    [[nodiscard]] std::string_view getName() const {
        return this->providerName;
    }
    [[nodiscard]] virtual bool hasResource(std::string_view name) const = 0;
    virtual void compileResource(std::string_view name, Resource* resource) const = 0;
protected:
    std::string providerName;
};

} // namespace chira
