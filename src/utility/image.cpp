#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

image::image(const std::string &filepath, int* width, int* height, int* fileChannels, int desiredChannels) {
    this->data = stbi_load(filepath.c_str(), width, height, fileChannels, desiredChannels);
}

image::~image() {
    if (this->data) {
        stbi_image_free(this->data);
    }
}

unsigned char* image::getData() {
    return this->data;
}
