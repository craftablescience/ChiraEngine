#pragma once

#include <functional>
#include <string>
#include <resource/PropertiesResource.h>
#include <render/shader/Shader.h>
#include <utility/AbstractFactory.h>

namespace chira {

class IMaterial : public PropertiesResource {
public:
    explicit IMaterial(std::string identifier_);
    void compile(const nlohmann::json& properties) override;
    virtual void use() const;
    [[nodiscard]] SharedPointer<Shader> getShader() const;
    void setShader(std::string path);
protected:
    SharedPointer<Shader> shader;
    std::string shaderPath{"file://shaders/unlitTextured.json"};
public:
    CHIRA_PROPS() (
            CHIRA_PROP_NAMED_SET(IMaterial, shaderPath, shader, setShader)
    );
};

using MaterialFactory = AbstractFactory<SharedPointer<IMaterial>>;

} // namespace chira

#define CHIRA_REGISTER_MATERIAL_TYPE(ResourceClassName)                                         \
    static inline const bool ResourceClassName##FactoryRegistryHelper =                         \
        chira::MaterialFactory::registerTypeFactory(                                            \
            #ResourceClassName,                                                                 \
            [](const std::string& materialId) -> chira::SharedPointer<chira::IMaterial> {       \
                return chira::Resource::getResource<ResourceClassName>(materialId)              \
                       .castAssert<chira::IMaterial>();                                         \
            }                                                                                   \
        )

#define CHIRA_GET_MATERIAL(type, identifier) \
    chira::MaterialFactory::getTypeFactory(type)(identifier).castAssert<chira::IMaterial>()
