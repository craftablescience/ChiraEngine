#pragma once

#include "Resource.h"
#include <loader/image/Image.h>

namespace chira {

class TextureResource : public Resource {
public:
    explicit TextureResource(std::string identifier_, bool vFlip_ = true);
    void compile(const unsigned char buffer[], std::size_t bufferLen) override;
    [[nodiscard]] Image* getFile() const {
        return this->file.get();
    }
    [[nodiscard]] unsigned char* getData() const {
        return this->file->getData();
    }
    [[nodiscard]] int getWidth() const {
        return this->width;
    }
    [[nodiscard]] int getHeight() const {
        return this->height;
    }
    [[nodiscard]] int getBitDepth() const {
        return this->bitDepth;
    }
    [[nodiscard]] bool isVerticallyFlipped() const {
        return this->vFlip;
    }
protected:
    std::unique_ptr<Image> file = nullptr;
    int width = -1;
    int height = -1;
    int bitDepth = -1;
    bool vFlip = true;
private:
    CHIRA_REGISTER_DEFAULT_RESOURCE(TextureResource, "file://textures/missing.png"); // NOLINT(cert-err58-cpp)
};

} // namespace chira
