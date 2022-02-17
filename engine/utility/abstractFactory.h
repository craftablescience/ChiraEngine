#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace chira {
    template<typename ReturnType>
    class AbstractFactory {
        using factoryFunction = std::function<ReturnType(const std::string&)>;
    public:
        AbstractFactory() = delete;
        virtual ~AbstractFactory() = default;
        static bool registerTypeFactory(const std::string& name, factoryFunction createFunc) {
            if (AbstractFactory::getFactoryMethods().count(name) > 0)
                return false;
            AbstractFactory::getFactoryMethods()[name] = createFunc;
            return true;
        }
        static const factoryFunction& getTypeFactory(const std::string& name) {
            return AbstractFactory::getFactoryMethods()[name];
        }
    protected:
        static std::unordered_map<std::string, factoryFunction>& getFactoryMethods() {
            static std::unordered_map<std::string, factoryFunction> factoryFunctions;
            return factoryFunctions;
        }
    };
}
