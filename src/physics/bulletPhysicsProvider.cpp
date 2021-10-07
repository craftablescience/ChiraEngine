#include "bulletPhysicsProvider.h"

using namespace chira;

bulletPhysicsProvider::bulletPhysicsProvider() {
    this->collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    this->dispatcher = std::make_unique<btCollisionDispatcher>(this->collisionConfiguration.get());
    this->overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    this->solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    this->dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
            this->dispatcher.get(),
            this->overlappingPairCache.get(),
            this->solver.get(),
            this->collisionConfiguration.get());
    // Don't use bulletPhysicsProvider::setGravity because virtual method shouldn't be used in constructor
    this->dynamicsWorld->setGravity(btVector3(0, -12, 0));
}

void bulletPhysicsProvider::updatePhysics(double delta) {
    this->dynamicsWorld->stepSimulation((btScalar) delta, 4);
}

void bulletPhysicsProvider::stop() {
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

void bulletPhysicsProvider::addRigidBody(btRigidBody* rb) {
    this->dynamicsWorld->addRigidBody(rb);
}

void bulletPhysicsProvider::setGravity(const glm::vec3& gravity) {
    this->dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}
