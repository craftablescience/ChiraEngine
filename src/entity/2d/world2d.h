#pragma once

#include "entity2d.h"

namespace chira {
    class world2d : public entity2d {
    public:
        explicit world2d(entity* parent_) : entity2d(parent_) {}
        world2d(entity* parent_, const std::string& name_) : entity2d(parent_, name_) {}
        world2d() : entity2d() {}
        explicit world2d(const std::string& name_) : entity2d(name_) {}
    };
}
