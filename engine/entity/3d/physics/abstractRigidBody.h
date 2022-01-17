#pragma once

#include "../entity3d.h"

namespace chira {
    class AbstractRigidBody : public Entity3d {
    public:
        explicit AbstractRigidBody(Entity* parent_) : Entity3d(parent_) {}
        AbstractRigidBody(Entity* parent_, const std::string& name_) : Entity3d(parent_, name_) {}
        AbstractRigidBody() : Entity3d() {}
        explicit AbstractRigidBody(const std::string& name_) : Entity3d(name_) {}
        virtual void onUserInteract() {};
    };
}
