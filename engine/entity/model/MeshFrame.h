#pragma once

#include <entity/root/Frame.h>
#include "MeshDynamic.h"

namespace chira {

class MeshFrame : public Frame {
public:
    MeshFrame(std::string name_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
    MeshFrame(int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
    void update(glm::mat4 parentTransform) override;
    [[nodiscard]] MeshDynamic* getMeshDynamic();
protected:
    MeshDynamic mesh{};
};

}
