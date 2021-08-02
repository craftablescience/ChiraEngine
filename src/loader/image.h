#pragma once

#include <string>
#include "abstractImage.h"

class image : public abstractImage {
public:
    image(const std::string& filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip = true);
    ~image();
};
