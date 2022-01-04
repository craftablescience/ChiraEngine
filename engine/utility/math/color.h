#pragma once

namespace chira {
    struct ColorR {
        float r;
        explicit ColorR(float R = 0.f);
    };

    struct ColorRG : public ColorR {
        float g;
        explicit ColorRG(float R = 0.f, float G = 0.f);
    };

    struct ColorRGB : public ColorRG {
        float b;
        explicit ColorRGB(float R = 0.f, float G = 0.f, float B = 0.f);
        static ColorRGB solid(float all);
    };

    struct ColorRGBA : public ColorRGB {
        float a;
        explicit ColorRGBA(float R = 0.f, float G = 0.f, float B = 0.f, float A = 1.f);
        static ColorRGBA solid(float all, float A);
    };
}
