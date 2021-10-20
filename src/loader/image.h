#pragma once

#include <string>
#include "abstractImage.h"

namespace chira {
    class image : public abstractImage {
    public:
        image(const unsigned char buffer[], std::size_t bufferLen, int* width, int* height, int* fileChannels, int desiredChannels = 0, bool vflip = true);
        image(const unsigned char buffer[], std::size_t bufferLen, int desiredChannels = 0, bool vflip = true);
        image(const std::string& filepath, int* width, int* height, int* fileChannels, int desiredChannels = 0, bool vflip = true);
        explicit image(const std::string& filepath, int desiredChannels = 0, bool vflip = true);
        ~image();
    };
}
