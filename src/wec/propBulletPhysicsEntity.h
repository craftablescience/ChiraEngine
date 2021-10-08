#pragma once

#include "abstractEntity.h"
#include "meshComponent.h"
#include "componentFactory.h"
#include "../physics/bulletRigidBodyComponent.h"

namespace chira {
    class propBulletPhysicsEntity : public abstractEntity {
    public:
        propBulletPhysicsEntity* init(meshComponent* mesh_, bulletRigidBodyComponent* physics_);
        void preRender(double delta) override;
        void postRender(double delta) override {}
    private:
        meshComponent* meshPtr = nullptr;
        bulletRigidBodyComponent* physics = nullptr;

        RegisterComponentName(propBulletPhysicsEntity);
    };
}
