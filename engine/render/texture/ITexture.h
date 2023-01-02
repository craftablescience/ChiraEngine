#pragma once

#include <string>

#include <resource/PropertiesResource.h>
#include <render/backend/RenderBackend.h>
#include <render/backend/RenderTypes.h>

namespace chira {

class ITexture : public PropertiesResource {
public:
    explicit ITexture(std::string identifier_) : PropertiesResource(std::move(identifier_)) {}

    virtual void use() const = 0;
    virtual void use(TextureUnit activeTextureUnit) const = 0;
protected:
    Renderer::TextureHandle handle{};
};

} // namespace chira
