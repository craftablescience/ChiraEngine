#pragma once

#include <functional>
#include <string>
#include <resource/propertiesResource.h>
#include "../shader.h"

namespace chira {
    class baseMaterial : public propertiesResource {
    public:
        explicit baseMaterial(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        virtual void use();
        sharedPointer<shader> getShader();
    protected:
        sharedPointer<shader> shaderPtr;
    };

    class materialFactory {
        using factoryFunction = std::function<sharedPointer<baseMaterial>(const std::string&)>;
    public:
        materialFactory() = delete;
        static bool registerMaterialType(const std::string& name, factoryFunction createFunc);
        static const factoryFunction& getMaterialType(const std::string& name);
    private:
        static inline std::unordered_map<std::string, factoryFunction> factoryMethods;
    };
}

#define REGISTER_MATERIAL_TYPE(resourceClassName)                                              \
    static inline bool resourceClassName##FactoryRegistryHelper =                              \
    chira::materialFactory::registerMaterialType(                                              \
        #resourceClassName,                                                                    \
        [](const std::string& identifier) -> chira::sharedPointer<chira::baseMaterial> {       \
            return chira::resource::getResource<resourceClassName>(identifier)                 \
                   .castReinterpret<chira::baseMaterial>();                                    \
        }                                                                                      \
    )

#define GET_MATERIAL(type, identifier) chira::materialFactory::getMaterialType(type)(identifier).castReinterpret<chira::baseMaterial>()
