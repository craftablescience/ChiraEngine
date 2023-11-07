#pragma once

#include <functional>
#include <string>

#include <core/utility/AbstractFactory.h>
#include <core/utility/Serial.h>
#include <render/shader/Shader.h>
#include <resource/Resource.h>

namespace chira {

class IMaterial : public Resource {
public:
    explicit IMaterial(std::string identifier_);
    void compile(const byte buffer[], std::size_t bufferLength) override;
    virtual void use() const;
    [[nodiscard]] SharedPointer<Shader> getShader() const;

protected:
    SharedPointer<Shader> shader;
    std::string shaderPath{"file://shaders/unlitTextured.json"};

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("shader", this->shaderPath)
        );
    }
};

using MaterialFactory = AbstractFactory<SharedPointer<IMaterial>>;

} // namespace chira

#define CHIRA_REGISTER_MATERIAL_TYPE(ResourceClassName)                                      \
    static inline const bool ResourceClassName##FactoryRegistryHelper =                      \
        chira::MaterialFactory::registerTypeFactory(                                         \
            #ResourceClassName,                                                              \
            [](const std::string& materialId) -> chira::SharedPointer<chira::IMaterial> {    \
                return chira::Resource::getResource<ResourceClassName>(materialId)           \
                       .cast<chira::IMaterial>();                                            \
            }                                                                                \
        )

#define CHIRA_GET_MATERIAL(type, identifier) \
    chira::MaterialFactory::getTypeFactory(type)(identifier)
