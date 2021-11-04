#pragma once

#include "entity3d.h"

namespace chira {
    class world3d : public entity3d {
    public:
        explicit world3d(entity* parent_) : entity3d(parent_) {}
        world3d(entity* parent_, const std::string& name_) : entity3d(parent_, name_) {}
        world3d() : entity3d() {}
        explicit world3d(const std::string& name_) : entity3d() {}
    };
}
