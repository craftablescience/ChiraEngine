#include "color.h"

#include <algorithm>

using namespace chira;

ColorR::ColorR(float R) {
    this->r = std::clamp(R, 0.f, 1.f);
}

ColorRG::ColorRG(float R, float G) : ColorR(R) {
    this->g = std::clamp(G, 0.f, 1.f);
}

ColorRGB::ColorRGB(float R, float G, float B) : ColorRG(R,G) {
    this->b = std::clamp(B, 0.f, 1.f);
}

ColorRGB ColorRGB::solid(float all) {
    return ColorRGB{all, all, all};
}

ColorRGBA::ColorRGBA(float R, float G, float B, float A) : ColorRGB(R,G,B) {
    this->a = std::clamp(A, 0.f, 1.f);
}

ColorRGBA ColorRGBA::solid(float all, float A) {
    return ColorRGBA{all, all, all, A};
}
