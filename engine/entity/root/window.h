#pragma once

#include "frame.h"

namespace chira {
    class Window : public Frame {
    public:
        Window(const std::string& name_, unsigned int width_, unsigned int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        Window(unsigned int width_, unsigned int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
    };
}
