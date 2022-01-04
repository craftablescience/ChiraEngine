#pragma once

#include "entity3d.h"

namespace chira {
    class World3d : public Entity3d {
    public:
        explicit World3d(Entity* parent_) : Entity3d(parent_) {}
        World3d(Entity* parent_, const std::string& name_) : Entity3d(parent_, name_) {}
        World3d() : Entity3d() {}
        explicit World3d(const std::string& name_) : Entity3d(name_) {}
    };
}
