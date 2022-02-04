#pragma once

#include <entity/entity.h>
#include <utility/uuidGenerator.h>

namespace chira {
    class AbstractRigidBody : public Entity {
    public:
        explicit AbstractRigidBody(const std::string& name_) : Entity(name_) {}
        AbstractRigidBody() : Entity() {}
        virtual void onUserInteract() {};
    };
}
