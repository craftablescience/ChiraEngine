#pragma once

#include <resource/propertiesResource.h>
#include <btBulletDynamicsCommon.h>

namespace chira {
    enum class BulletColliderType {
        BULLET_INVALID,
        BULLET_BOX,
        BULLET_BVH_TRIANGLE_MESH,
        BULLET_CAPSULE, // can be X, Y, or Z
        BULLET_COMPOUND,
        BULLET_CONE, // can be X, Y, or Z
        BULLET_CONVEX_HULL,
        BULLET_CONVEX_POINT_CLOUD,
        BULLET_CONVEX_POLYHEDRON,
        BULLET_CONVEX_TRIANGLE_MESH,
        BULLET_CYLINDER, // can be X, Y, or Z
        BULLET_HEIGHTFIELD_TERRAIN,
        BULLET_MULTIMATERIAL_TRIANGLE_MESH,
        BULLET_MULTISPHERE,
        BULLET_SDF_COLLISION,
        BULLET_SPHERE,
        BULLET_STATIC_PLANE
    };

    /// Note: this does not add or remove the collider to the physics world.
    /// This must be done by a component.
    class BulletColliderResource : public PropertiesResource {
    public:
        explicit BulletColliderResource(const std::string& identifier_) : PropertiesResource(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        btRigidBody* getNewRigidBody();
        static BulletColliderType getBulletColliderTypeFromString(const std::string& colliderTypeStr);
    private:
        std::unique_ptr<btCollisionShape> collider;
        BulletColliderType colliderType = BulletColliderType::BULLET_INVALID;
        btScalar mass = 0.0;
        btTransform transform;
        btVector3 localInertia;
        btScalar originX = 0.0;
        btScalar originY = 0.0;
        btScalar originZ = 0.0;
    };
}
