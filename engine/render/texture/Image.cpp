#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace chira;

Image::Image(std::string identifier_, bool vFlip_)
    : Resource(std::move(identifier_))
    , vFlip(vFlip_) {}

Image::~Image() {
    Image::freeUncompressedImage(this->data);
}

void Image::compile(const byte buffer[], std::size_t bufferLen) {
    int w, h, bd;
    this->data = Image::getUncompressedImage(buffer, static_cast<int>(bufferLen) - 1, &w, &h, &bd, 0, this->vFlip);
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
}

byte* Image::getUncompressedImage(const byte buffer[], int bufferLen, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip) {
    stbi_set_flip_vertically_on_load(vflip);
    return stbi_load_from_memory(buffer, bufferLen, width, height, fileChannels, desiredChannels);
}

byte* Image::getUncompressedImage(const byte buffer[], int bufferLen, int desiredChannels, bool vflip) {
    int width, height, fileChannels;
    return Image::getUncompressedImage(buffer, bufferLen, &width, &height, &fileChannels, desiredChannels, vflip);
}

byte* Image::getUncompressedImage(std::string_view filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip) {
    stbi_set_flip_vertically_on_load(vflip);
    return stbi_load(filepath.data(), width, height, fileChannels, desiredChannels);
}

byte* Image::getUncompressedImage(std::string_view filepath, int desiredChannels, bool vflip) {
    int width, height, fileChannels;
    return Image::getUncompressedImage(filepath, &width, &height, &fileChannels, desiredChannels, vflip);
}

void Image::freeUncompressedImage(byte* image) {
    if (image) {
        stbi_image_free(image);
    }
}
