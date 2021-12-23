#include "color.h"

#include <algorithm>

using namespace chira;

colorR::colorR(float R) {
    this->r = std::clamp(R, 0.f, 1.f);
}

colorRG::colorRG(float R, float G) : colorR(R) {
    this->g = std::clamp(G, 0.f, 1.f);
}

colorRGB::colorRGB(float R, float G, float B) : colorRG(R,G) {
    this->b = std::clamp(B, 0.f, 1.f);
}

colorRGB colorRGB::solid(float all) {
    return colorRGB{all, all, all};
}

colorRGBA::colorRGBA(float R, float G, float B, float A) : colorRGB(R,G,B) {
    this->a = std::clamp(A, 0.f, 1.f);
}

colorRGBA colorRGBA::solid(float all, float A) {
    return colorRGBA{all, all, all, A};
}
