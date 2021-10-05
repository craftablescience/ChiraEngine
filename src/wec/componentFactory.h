#pragma once

#include "abstractComponent.h"

namespace chira {
    class componentFactory {
    public:
        static abstractComponent* getComponent(const std::string& name) {
            return componentFactory::componentTypes[name]();
        }
    protected:
        static inline std::unordered_map<std::string,abstractComponent*(*)()> componentTypes{};

        template<typename T>
        static abstractComponent* createType() {
            return new T;
        }
    };

    template<typename T>
    struct componentRegistry : public componentFactory {
        explicit componentRegistry(const std::string& name) {
            componentFactory::componentTypes[name] = &createType<T>;
        }
    };
}

#define RegisterComponentName(name) static inline componentRegistry<name> name##RegistryEntry{#name} // NOLINT(cert-err58-cpp)
