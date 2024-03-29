#pragma once

#include <string>

#include <resource/Resource.h>
#include <render/backend/RenderBackend.h>
#include <render/backend/RenderTypes.h>

namespace chira {

class ITexture : public Resource {
public:
    explicit ITexture(std::string identifier_)
            : Resource(std::move(identifier_)) {}

    virtual void use() const = 0;
    virtual void use(TextureUnit activeTextureUnit) const = 0;

protected:
    Renderer::TextureHandle handle{};
};

} // namespace chira
