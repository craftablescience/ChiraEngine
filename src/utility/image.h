#ifndef BASICGAMEENGINE_IMAGE_H
#define BASICGAMEENGINE_IMAGE_H


#include <string>

class image {
public:
    image(const std::string& filepath, int* width, int* height, int* fileChannels, int desiredChannels);
    ~image();
    [[nodiscard]] unsigned char* getData();
protected:
    unsigned char* data;
};


#endif //BASICGAMEENGINE_IMAGE_H
