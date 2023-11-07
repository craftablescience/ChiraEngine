#pragma once

#include <core/math/Types.h>
#include <resource/Resource.h>

namespace chira {

class Image : public Resource {
public:
    explicit Image(std::string identifier_, bool vFlip = true);
    ~Image() override;
    Image(const Image& other) = delete;
    Image& operator=(const Image& other) = delete;
    Image(Image&& other) noexcept = default;
    Image& operator=(Image&& other) noexcept = default;

    void compile(const byte buffer[], std::size_t bufferLen) override;
    [[nodiscard]] inline byte* getData() const {
        return this->image;
    }
    [[nodiscard]] inline int getWidth() const {
        return this->width;
    }
    [[nodiscard]] inline int getHeight() const {
        return this->height;
    }
    [[nodiscard]] inline int getBitDepth() const {
        return this->bitDepth;
    }
    [[nodiscard]] inline bool isVerticallyFlipped() const {
        return this->verticalFlip;
    }

    [[nodiscard]] static byte* getUncompressedImage(const byte buffer[], int bufferLen, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip);
    [[nodiscard]] static byte* getUncompressedImage(const byte buffer[], int bufferLen, int desiredChannels, bool vflip);
    [[nodiscard]] static byte* getUncompressedImage(std::string_view filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip);
    [[nodiscard]] static byte* getUncompressedImage(std::string_view filepath, int desiredChannels, bool vflip);
    static void deleteUncompressedImage(byte* image);
protected:
    byte* image = nullptr;
    int width = -1;
    int height = -1;
    int bitDepth = -1;
    bool verticalFlip = true;
private:
    CHIRA_REGISTER_DEFAULT_RESOURCE(Image, "file://textures/missing.png");
};

} // namespace chira
