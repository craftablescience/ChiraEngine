#pragma once

#include <memory>
#include <string>
#include "../resource/abstractMetaResource.h"
#include "../loader/abstractImage.h"

class texture : public abstractResource {
public:
    texture(const std::string& provider_, const std::string& name_);
    void compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLen) override;
    void compile(unsigned char* buffer, unsigned int bufferLen);
    virtual void use() = 0;
    void setTextureUnit(int textureUnit);
    [[nodiscard]] int getTextureUnit() const;
    [[nodiscard]] unsigned int getHandle() const;
protected:
    unsigned int handle = 0;
    int activeTextureUnit = -1;
    std::unique_ptr<abstractImage> file = nullptr;
    int width = -1;
    int height = -1;
    int bitDepth = -1;
};
