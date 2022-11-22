#pragma once

#include <string>

#include <resource/PropertiesResource.h>
#include <render/backend/RendererTypes.h>
#include <utility/HandleObject.h>

namespace chira {

class ITexture : public PropertiesResource, public HandleObject<unsigned int> {
public:
    explicit ITexture(std::string identifier_)
            : PropertiesResource(std::move(identifier_))
            , HandleObject<unsigned int>(0) {}

    virtual void use() const = 0;
    virtual void use(TextureUnit activeTextureUnit) const = 0;
};

} // namespace chira
