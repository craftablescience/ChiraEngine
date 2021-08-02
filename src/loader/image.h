#pragma once

#include <string>

class image {
public:
    image(const std::string& filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip = true);
    virtual ~image();
    [[nodiscard]] virtual unsigned char* getData();
protected:
    unsigned char* data;
};
