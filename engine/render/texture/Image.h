#pragma once

#include <math/Types.h>
#include <resource/Resource.h>

namespace chira {

class Image : public Resource {
public:
    explicit Image(std::string identifier_, bool vFlip_ = true);
    ~Image() override;
    void compile(const byte buffer[], std::size_t bufferLen) override;
    [[nodiscard]] byte* getData() const {
        return this->data;
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

    [[nodiscard]] static byte* getUncompressedImage(const byte buffer[], int bufferLen, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip);
    [[nodiscard]] static byte* getUncompressedImage(const byte buffer[], int bufferLen, int desiredChannels, bool vflip);
    [[nodiscard]] static byte* getUncompressedImage(std::string_view filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip);
    [[nodiscard]] static byte* getUncompressedImage(std::string_view filepath, int desiredChannels, bool vflip);
    static void freeUncompressedImage(byte* image);
protected:
    byte* data = nullptr;
    int width = -1;
    int height = -1;
    int bitDepth = -1;
    bool vFlip = true;
private:
    CHIRA_REGISTER_DEFAULT_RESOURCE(Image, "file://textures/missing.png"); // NOLINT(cert-err58-cpp)
};

} // namespace chira
