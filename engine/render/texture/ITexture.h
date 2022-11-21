#pragma once

#include <string>

#include <resource/PropertiesResource.h>
#include <utility/HandleObject.h>

namespace chira {

class ITexture : public PropertiesResource, public HandleObject<unsigned int> {
public:
    explicit ITexture(std::string identifier_)
            : PropertiesResource(std::move(identifier_))
            , HandleObject<unsigned int>(0) {}

    virtual void use() = 0;

    void setTextureUnit(int textureUnit) {
        this->activeTextureUnit = textureUnit;
    }

    [[nodiscard]] int getTextureUnit() const {
        return this->activeTextureUnit;
    }
protected:
    int activeTextureUnit = -1;
};

} // namespace chira
