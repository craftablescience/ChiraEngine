#include "color.h"

#include <algorithm>

using namespace chira;

ColorR::ColorR(float R)
    : r(std::clamp(R, 0.f, 1.f)) {}
ColorR::ColorR() : ColorR(0) {}

ColorRG::ColorRG(float R, float G)
    : r(std::clamp(R, 0.f, 1.f))
    , g(std::clamp(G, 0.f, 1.f)) {}
ColorRG::ColorRG(float all) : ColorRG(all, all) {}
ColorRG::ColorRG() : ColorRG(0) {}

ColorRGB::ColorRGB(float R, float G, float B)
    : r(std::clamp(R, 0.f, 1.f))
    , g(std::clamp(G, 0.f, 1.f))
    , b(std::clamp(B, 0.f, 1.f)) {}
ColorRGB::ColorRGB(float all) : ColorRGB(all, all, all) {}
ColorRGB::ColorRGB() : ColorRGB(0) {}

ColorRGBA::ColorRGBA(float R, float G, float B, float A)
    : r(std::clamp(R, 0.f, 1.f))
    , g(std::clamp(G, 0.f, 1.f))
    , b(std::clamp(B, 0.f, 1.f))
    , a(std::clamp(A, 0.f, 1.f)) {}
ColorRGBA::ColorRGBA(float all, float A) : ColorRGBA(all, all, all, A) {}
ColorRGBA::ColorRGBA() : ColorRGBA(0) {}
