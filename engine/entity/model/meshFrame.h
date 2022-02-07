#pragma once

#include <entity/root/frame.h>
#include "meshDynamic.h"

namespace chira {
    class MeshFrame : public Frame {
    public:
        MeshFrame(const std::string& name_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        MeshFrame(int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        void render(glm::mat4 parentTransform) override;
        [[nodiscard]] MeshDynamic* getMeshDynamic();
    protected:
        MeshDynamic mesh{};
    };
}
