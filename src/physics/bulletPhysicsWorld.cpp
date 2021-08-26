#include "bulletPhysicsWorld.h"

bulletPhysicsWorld::bulletPhysicsWorld() {
    this->collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    this->dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
    this->overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    this->solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    this->dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
            dispatcher.get(),
            overlappingPairCache.get(),
            solver.get(),
            collisionConfiguration.get());
    this->dynamicsWorld->setGravity(btVector3(0, -12, 0));
}

void bulletPhysicsWorld::updatePhysics(double delta) {
    this->dynamicsWorld->stepSimulation((btScalar) delta, 4);
}

void bulletPhysicsWorld::stop() {
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

unsigned int bulletPhysicsWorld::addBoxCollider(const glm::vec3& bounds) {

}

void bulletPhysicsWorld::removeCollider(btCollisionShape* collider) {

}

void bulletPhysicsWorld::setGravity(const glm::vec3& gravity) {
    this->dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}
