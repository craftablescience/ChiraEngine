#pragma once

#include "abstractPhysicsProvider.h"
#include "bulletColliderResource.h"
#include <memory>
#include <vector>

namespace chira {
    class bulletPhysicsProvider : public abstractPhysicsProvider {
    public:
        bulletPhysicsProvider();
        void updatePhysics(double delta) override;
        void stop() override;
        // todo: there must be a way to remove a RigidBody
        void addRigidBody(btRigidBody* rb);
        void setGravity(const glm::vec3& gravity) override;
    private:
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> dispatcher;
        std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
    };
}
