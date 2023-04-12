#pragma once

namespace chira {

struct ColorR {
    float r;
    ColorR(float R); // NOLINT(google-explicit-constructor)
    ColorR();
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
    ColorRG(float R, float G);
    ColorRG(float all); // NOLINT(google-explicit-constructor)
    ColorRG();
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
    ColorRGB(float R, float G, float B);
    ColorRGB(float all); // NOLINT(google-explicit-constructor)
    ColorRGB();
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
    ColorRGBA(float R, float G, float B, float A = 1.f);
    ColorRGBA(float all, float A = 1.f); // NOLINT(google-explicit-constructor)
    ColorRGBA(ColorRGB color, float A = 1.f); // NOLINT(google-explicit-constructor)
    ColorRGBA();
    bool operator==(const ColorRGBA& other) const {
        return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
    }
    bool operator!=(const ColorRGBA& other) const {
        return !this->operator==(other);
    }
};

} // namespace chira
