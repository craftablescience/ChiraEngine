#pragma once

#include <memory>
#include <string>
#include "../loader/abstractImage.h"

class texture {
public:
    explicit texture(const std::string& file);
    texture(abstractImage* image, int w, int h, int bd);
    void compile();
    virtual void use() = 0;
    void setTextureUnit(int textureUnit);
    [[nodiscard]] int getTextureUnit() const;
    [[nodiscard]] unsigned int getHandle() const;
protected:
    unsigned int handle = 0;
    int activeTextureUnit = -1;
    std::unique_ptr<abstractImage> file = nullptr;
    int width;
    int height;
    int bitDepth;
};
