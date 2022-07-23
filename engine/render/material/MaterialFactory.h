#pragma once

#include <functional>
#include <string>
#include <resource/PropertiesResource.h>
#include <render/Shader.h>
#include <utility/AbstractFactory.h>

namespace chira {

class MaterialBase : public PropertiesResource {
public:
    explicit MaterialBase(std::string identifier_);
    void compile(const nlohmann::json& properties) override;
    virtual void use() const;
    [[nodiscard]] SharedPointer<Shader> getShader() const;
protected:
    SharedPointer<Shader> shader;
};

using MaterialFactory = AbstractFactory<SharedPointer<MaterialBase>>;

} // namespace chira

#define CHIRA_REGISTER_MATERIAL_TYPE(ResourceClassName)                                         \
    static inline const bool ResourceClassName##FactoryRegistryHelper =                         \
        chira::MaterialFactory::registerTypeFactory(                                            \
            #ResourceClassName,                                                                 \
            [](const std::string& materialId) -> chira::SharedPointer<chira::MaterialBase> {    \
                return chira::Resource::getResource<ResourceClassName>(materialId)              \
                       .castAssert<chira::MaterialBase>();                                      \
            }                                                                                   \
        )

#define CHIRA_GET_MATERIAL(type, identifier) \
    chira::MaterialFactory::getTypeFactory(type)(identifier).castAssert<chira::MaterialBase>()
