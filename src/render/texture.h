#pragma once

#include <memory>
#include "../loader/image.h"
#include "../utility/compilable.h"

class texture : public compilable {
public:
    explicit texture(const std::string& file);
    void compile() override;
    virtual void use() = 0;
    void setTextureUnit(int textureUnit);
    [[nodiscard]] int getTextureUnit() const;
    [[nodiscard]] unsigned int getHandle() const;
protected:
    unsigned int handle = 0;
    int activeTextureUnit = -1;
    std::unique_ptr<image> file = nullptr;
    int width;
    int height;
    int bitDepth;
};
