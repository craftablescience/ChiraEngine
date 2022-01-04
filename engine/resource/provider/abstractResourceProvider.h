#pragma once

namespace chira {
    class Resource;

    class AbstractResourceProvider {
    public:
        explicit AbstractResourceProvider(std::string name) : providerName(std::move(name)) {}
        virtual ~AbstractResourceProvider() = default;
        const std::string& getName() {
            return this->providerName;
        }
        virtual bool hasResource(const std::string& name) = 0;
        virtual void compileResource(const std::string& name, Resource* resource) = 0;
    protected:
        std::string providerName;
    };
}
