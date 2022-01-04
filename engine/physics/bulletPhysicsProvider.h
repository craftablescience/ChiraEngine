#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "abstractPhysicsProvider.h"

namespace chira {
    class BulletPhysicsProvider : public AbstractPhysicsProvider {
    public:
        BulletPhysicsProvider();
        void updatePhysics(double delta) override;
        void stop() override;
        void addRigidBody(btRigidBody* rb);
        void removeRigidBody(btRigidBody* rb);
        void setGravity(glm::vec3 gravity) override;
    private:
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> dispatcher;
        std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
    };
}
