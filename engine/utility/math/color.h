#pragma once

namespace chira {
    struct colorR {
        float r;
        explicit colorR(float R = 0.f);
    };

    struct colorRG : public colorR {
        float g;
        explicit colorRG(float R = 0.f, float G = 0.f);
    };

    struct colorRGB : public colorRG {
        float b;
        explicit colorRGB(float R = 0.f, float G = 0.f, float B = 0.f);
        static colorRGB solid(float all);
    };

    struct colorRGBA : public colorRGB {
        float a;
        explicit colorRGBA(float R = 0.f, float G = 0.f, float B = 0.f, float A = 1.f);
        static colorRGBA solid(float all, float A);
    };
}
