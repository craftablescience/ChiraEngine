#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace chira {

template<typename ReturnType>
class IFactory {
    using factoryFunction = std::function<ReturnType(const std::string&)>;
public:
    IFactory() = delete;
    virtual ~IFactory() = default;
    static bool registerTypeFactory(const std::string& name, factoryFunction createFunc) {
        if (IFactory::getFactoryMethods().count(name) > 0)
            return false;
        IFactory::getFactoryMethods()[name] = createFunc;
        return true;
    }
    static const factoryFunction& getTypeFactory(const std::string& name) {
        return IFactory::getFactoryMethods()[name];
    }
protected:
    static std::unordered_map<std::string, factoryFunction>& getFactoryMethods() {
        static std::unordered_map<std::string, factoryFunction> factoryFunctions;
        return factoryFunctions;
    }
};

} // namespace chira
