#pragma once

#include <string>

namespace chira {

class Resource;

class IResourceProvider {
public:
    explicit IResourceProvider(std::string name) : providerName(std::move(name)) {}
    virtual ~IResourceProvider() = default;
    [[nodiscard]] const std::string& getName() const {
        return this->providerName;
    }
    [[nodiscard]] virtual bool hasResource(const std::string& name) const = 0;
    virtual void compileResource(const std::string& name, Resource* resource) const = 0;
protected:
    std::string providerName;
};

}
