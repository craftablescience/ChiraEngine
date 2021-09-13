#pragma once

#include "abstractPhysicsProvider.h"
#include "bulletColliderResource.h"
#include <memory>
#include <vector>

class bulletColliderResource;

class bulletPhysicsProvider : public abstractPhysicsProvider {
public:
    bulletPhysicsProvider();
    void updatePhysics(double delta) override;
    void stop() override;
    bulletColliderResource* addBoxCollider(const glm::vec3& bounds);
    void removeCollider(btCollisionShape* collider);
    void setGravity(const glm::vec3& gravity) override;
private:
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
};
