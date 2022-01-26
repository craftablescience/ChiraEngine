#pragma once

#include <entity/entity.h>
#include <utility/uuidGenerator.h>

namespace chira {
    class AbstractRigidBody : public Entity {
    public:
        explicit AbstractRigidBody(Entity* parent_) : Entity(parent_) {}
        AbstractRigidBody(Entity* parent_, const std::string& name_) : Entity(parent_, name_) {}
        AbstractRigidBody() : Entity() {}
        explicit AbstractRigidBody(const std::string& name_) : Entity(name_) {}
        virtual void onUserInteract() {};
    };
}
