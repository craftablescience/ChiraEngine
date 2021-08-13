#pragma once

#include <memory>
#include <string>
#include "../loader/abstractImage.h"
#include "../utility/compilable.h"

class texture : public compilable {
public:
    explicit texture(const std::string& file);
    texture(abstractImage* image, int w, int h, int bd);
    void compile() override;
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
