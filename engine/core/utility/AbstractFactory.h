#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace chira {

// check render/material/MaterialFactory.h:L21 for example usage

template<typename ReturnType>
class AbstractFactory {
    using factoryFunction = std::function<ReturnType(const std::string&)>;
public:
    AbstractFactory() = delete;
    static bool registerTypeFactory(const std::string& name, factoryFunction createFunc) {
        if (AbstractFactory::getFactoryMethods().contains(name))
            return false;
        AbstractFactory::getFactoryMethods()[name] = createFunc;
        return true;
    }
    static const factoryFunction& getTypeFactory(const std::string& name) {
        return AbstractFactory::getFactoryMethods().at(name);
    }
protected:
    static std::unordered_map<std::string, factoryFunction>& getFactoryMethods() {
        static std::unordered_map<std::string, factoryFunction> factoryFunctions;
        return factoryFunctions;
    }
};

} // namespace chira
