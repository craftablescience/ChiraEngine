#pragma once

#include <algorithm>

namespace chira {

struct ColorR {
    float r;
    ColorR(float R) // NOLINT(google-explicit-constructor)
            : r(std::clamp(R, 0.f, 1.f)) {}
    ColorR()
            : ColorR(0) {}
    bool operator==(const ColorR& other) const {
        return this->r == other.r;
    }
    bool operator!=(const ColorR& other) const {
        return !this->operator==(other);
    }
};

struct ColorRG {
    float r;
    float g;
    ColorRG(float R, float G)
            : r(std::clamp(R, 0.f, 1.f))
            , g(std::clamp(G, 0.f, 1.f)) {}
    ColorRG(float all) // NOLINT(google-explicit-constructor)
            : ColorRG(all, all) {}
    ColorRG()
            : ColorRG(0) {}
    bool operator==(const ColorRG& other) const {
        return this->r == other.r && this->g == other.g;
    }
    bool operator!=(const ColorRG& other) const {
        return !this->operator==(other);
    }
};

struct ColorRGB {
    float r;
    float g;
    float b;
    ColorRGB(float R, float G, float B)
            : r(std::clamp(R, 0.f, 1.f))
            , g(std::clamp(G, 0.f, 1.f))
            , b(std::clamp(B, 0.f, 1.f)) {}
    ColorRGB(float all) // NOLINT(google-explicit-constructor)
            : ColorRGB(all, all, all) {}
    ColorRGB()
            : ColorRGB(0) {}
    bool operator==(const ColorRGB& other) const {
        return this->r == other.r && this->g == other.g && this->b == other.b;
    }
    bool operator!=(const ColorRGB& other) const {
        return !this->operator==(other);
    }
};

struct ColorRGBA {
    float r;
    float g;
    float b;
    float a;
    ColorRGBA(float R, float G, float B, float A = 1.f)
            : r(std::clamp(R, 0.f, 1.f))
            , g(std::clamp(G, 0.f, 1.f))
            , b(std::clamp(B, 0.f, 1.f))
            , a(std::clamp(A, 0.f, 1.f)) {}
    ColorRGBA(float all, float A = 1.f) // NOLINT(google-explicit-constructor)
            : ColorRGBA(all, all, all, A) {}
    ColorRGBA(ColorRGB color, float A = 1.f) // NOLINT(google-explicit-constructor)
            : ColorRGBA(color.r, color.g, color.b, A) {}
    ColorRGBA()
            : ColorRGBA(0) {}
    bool operator==(const ColorRGBA& other) const {
        return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
    }
    bool operator!=(const ColorRGBA& other) const {
        return !this->operator==(other);
    }
};

} // namespace chira
