#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace chira;

Image::~Image() {
    if (this->image) {
        stbi_image_free(image);
    }
}

Image::Image(std::string path_, bool vFlip)
    : Resource(std::move(path_))
    , verticalFlip(vFlip) {}

void Image::compile(const std::byte buffer[], std::size_t bufferLen) {
    int w, h, bd;
    this->image = Image::getUncompressedImage(buffer, static_cast<int>(bufferLen) - 1, &w, &h, &bd, 0, this->isVerticallyFlipped());
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
}

std::byte* Image::getUncompressedImage(const std::byte buffer[], int bufferLen, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip) {
    stbi_set_flip_vertically_on_load(vflip);
    return reinterpret_cast<std::byte*>(stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(buffer), bufferLen, width, height, fileChannels, desiredChannels));
}

std::byte* Image::getUncompressedImage(const std::byte buffer[], int bufferLen, int desiredChannels, bool vflip) {
    int width, height, fileChannels;
    return Image::getUncompressedImage(buffer, bufferLen, &width, &height, &fileChannels, desiredChannels, vflip);
}

std::byte* Image::getUncompressedImage(std::string_view filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip) {
    stbi_set_flip_vertically_on_load(vflip);
    return reinterpret_cast<std::byte*>(stbi_load(filepath.data(), width, height, fileChannels, desiredChannels));
}

std::byte* Image::getUncompressedImage(std::string_view filepath, int desiredChannels, bool vflip) {
    int width, height, fileChannels;
    return Image::getUncompressedImage(filepath, &width, &height, &fileChannels, desiredChannels, vflip);
}

void Image::deleteUncompressedImage(std::byte* image) {
    if (image) {
        stbi_image_free(image);
    }
}
