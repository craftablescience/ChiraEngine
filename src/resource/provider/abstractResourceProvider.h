#pragma once

#include <string>
#include <memory>
#include "../resource.h"

namespace chira {
    class abstractResourceProvider {
    public:
        explicit abstractResourceProvider(std::string name) : providerName(std::move(name)) {}
        virtual ~abstractResourceProvider() = default;
        const std::string& getName() {
            return this->providerName;
        }
        virtual bool hasResource(const std::string& name) = 0;
        virtual void compileResource(const std::string& name, resource* resource) = 0;
    protected:
        std::string providerName;
    };
}
