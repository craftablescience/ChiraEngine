#include "bulletPhysicsProvider.h"

#include <core/engine.h>
#include <utility/math/bulletConversions.h>

using namespace chira;

BulletPhysicsProvider::BulletPhysicsProvider() {
    this->collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>(btDefaultCollisionConstructionInfo());
    this->dispatcher = std::make_unique<btCollisionDispatcher>(this->collisionConfiguration.get());
    this->overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    this->solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    this->dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
            this->dispatcher.get(),
            this->overlappingPairCache.get(),
            this->solver.get(),
            this->collisionConfiguration.get());
    // Don't use bulletPhysicsProvider::setGravity because virtual method shouldn't be used in constructor
    this->dynamicsWorld->setGravity(btVector3{0, -12, 0});
}

void BulletPhysicsProvider::updatePhysics(double delta) {
    int step = 4;
    Engine::getSettingsLoader()->getValue("physics", "subStep", &step);
    this->dynamicsWorld->stepSimulation((btScalar) delta, step);
}

void BulletPhysicsProvider::stop() {
    for (int i = this->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        this->dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }
}

void BulletPhysicsProvider::addRigidBody(btRigidBody* rb) const {
    this->dynamicsWorld->addRigidBody(rb);
}

void BulletPhysicsProvider::removeRigidBody(btRigidBody* rb) const {
    this->dynamicsWorld->removeRigidBody(rb);
}

void BulletPhysicsProvider::setGravity(const glm::vec3 gravity) {
    this->dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

AbstractRigidBody* BulletPhysicsProvider::traceRay(const glm::vec3 start, const glm::vec3 end) const {
    const btVector3 bStart = glmToBullet(start);
    const btVector3 bEnd   = glmToBullet(end);
    btCollisionWorld::ClosestRayResultCallback rayCallback{bStart, bEnd};
    this->dynamicsWorld->rayTest(bStart, bEnd, rayCallback);
    if (rayCallback.hasHit())
        return static_cast<AbstractRigidBody*>(rayCallback.m_collisionObject->getUserPointer());
    return nullptr;
}

AbstractRigidBody* BulletPhysicsProvider::traceRay(const glm::vec3 start, const glm::vec3 direction, const float magnitude) const {
    return this->traceRay(start, start + (direction * magnitude));
}
