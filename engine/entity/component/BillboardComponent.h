#pragma once

#include "TransformComponent.h"

namespace chira {

struct BillboardComponent {
    explicit BillboardComponent(bool xAxis = true, bool yAxis = true, bool zAxis = true)
            : x(xAxis)
            , y(yAxis)
            , z(zAxis) {}

public:
    TransformComponent* transform;
    bool x;
    bool y;
    bool z;
};

} // namespace chira
