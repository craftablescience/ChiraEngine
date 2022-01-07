#pragma once

namespace chira {
    class Resource;

    class AbstractResourceProvider {
    public:
        explicit AbstractResourceProvider(std::string name) : providerName(std::move(name)) {}
        virtual ~AbstractResourceProvider() = default;
        [[nodiscard]] const std::string& getName() const {
            return this->providerName;
        }
        [[nodiscard]] virtual bool hasResource(const std::string& name) const = 0;
        virtual void compileResource(const std::string& name, Resource* resource) const = 0;
    protected:
        std::string providerName;
    };
}
