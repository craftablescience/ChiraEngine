#pragma once

#include <functional>
#include <string>
#include <resource/propertiesResource.h>
#include "../shader.h"

namespace chira {
    class MaterialBase : public PropertiesResource {
    public:
        explicit MaterialBase(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        virtual void use();
        [[nodiscard]] SharedPointer<Shader> getShader() const;
    protected:
        SharedPointer<Shader> shader;
    };

    class MaterialFactory {
        using factoryFunction = std::function<SharedPointer<MaterialBase>(const std::string&)>;
    public:
        MaterialFactory() = delete;
        static bool registerMaterialType(const std::string& name, factoryFunction createFunc);
        static const factoryFunction& getMaterialType(const std::string& name);
    private:
        static inline std::unordered_map<std::string, factoryFunction> factoryMethods;
    };
}

#define REGISTER_MATERIAL_TYPE(ResourceClassName)                                              \
    static inline bool ResourceClassName##FactoryRegistryHelper =                              \
    chira::MaterialFactory::registerMaterialType(                                              \
        #ResourceClassName,                                                                    \
        [](const std::string& identifier) -> chira::SharedPointer<chira::MaterialBase> {       \
            return chira::Resource::getResource<ResourceClassName>(identifier)                 \
                   .castAssert<chira::MaterialBase>();                                         \
        }                                                                                      \
    )

#define GET_MATERIAL(type, identifier) chira::MaterialFactory::getMaterialType(type)(identifier).castAssert<chira::MaterialBase>()
