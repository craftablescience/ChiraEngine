#pragma once

#include <math/Types.h>
#include <resource/Resource.h>

namespace chira {

struct ImageData {
    explicit ImageData(byte* data_) : data(data_) {}
    ~ImageData();
    ImageData(const ImageData& other) = delete;
    ImageData& operator=(const ImageData& other) = delete;
    ImageData(ImageData&& other) noexcept {
        this->data = other.data;
        other.data = nullptr;
    }
    ImageData& operator=(ImageData&& other) noexcept {
        this->data = other.data;
        other.data = nullptr;
        return *this;
    }
    byte* data = nullptr;
};

class Image : public Resource {
public:
    explicit Image(std::string identifier_, bool vFlip_ = true);
    void compile(const byte buffer[], std::size_t bufferLen) override;
    [[nodiscard]] byte* getData() const {
        return this->image.data;
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

    [[nodiscard]] static ImageData getUncompressedImage(const byte buffer[], int bufferLen, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip);
    [[nodiscard]] static ImageData getUncompressedImage(const byte buffer[], int bufferLen, int desiredChannels, bool vflip);
    [[nodiscard]] static ImageData getUncompressedImage(std::string_view filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip);
    [[nodiscard]] static ImageData getUncompressedImage(std::string_view filepath, int desiredChannels, bool vflip);
protected:
    ImageData image{nullptr};
    int width = -1;
    int height = -1;
    int bitDepth = -1;
    bool vFlip = true;
private:
    CHIRA_REGISTER_DEFAULT_RESOURCE(Image, "file://textures/missing.png"); // NOLINT(cert-err58-cpp)
};

} // namespace chira
