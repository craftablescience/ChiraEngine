#pragma once

#include "../resource/propertiesResource.h"
#include "../utility/logger.h"
#include "../i18n/translationManager.h"
#include <fmt/core.h>
#include <btBulletDynamicsCommon.h>

namespace chira {
    enum bulletColliderType {
        BULLET_INVALID,
        BULLET_BOX_2D,
        BULLET_BOX,
        BULLET_BVH_TRIANGLE_MESH,
        BULLET_CAPSULE_X,
        BULLET_CAPSULE_Y,
        BULLET_CAPSULE_Z,
        BULLET_COMPOUND,
        BULLET_CONE_X,
        BULLET_CONE_Y,
        BULLET_CONE_Z,
        BULLET_CONVEX_2D,
        BULLET_CONVEX_HULL,
        BULLET_CONVEX_POINT_CLOUD,
        BULLET_CONVEX_POLYHEDRON,
        BULLET_CONVEX_TRIANGLE_MESH,
        BULLET_CYLINDER_X,
        BULLET_CYLINDER_Y,
        BULLET_CYLINDER_Z,
        BULLET_HEIGHTFIELD_TERRAIN,
        BULLET_MULTIMATERIAL_TRIANGLE_MESH,
        BULLET_MULTISPHERE,
        BULLET_SCALED_BVH_TRIANGLE_MESH,
        BULLET_SDF_COLLISION,
        BULLET_SPHERE,
        BULLET_STATIC_PLANE,
        BULLET_TETRAHEDRON,
        BULLET_UNIFORM_SCALING
    };

    /// Note: this does not add or remove the collider to the physics world.
    /// This must be done by a component.
    class bulletColliderResource : public propertiesResource {
    public:
        explicit bulletColliderResource(const std::string& identifier_) : propertiesResource(identifier_) {}
        void compile(const nlohmann::json& properties) override {
            this->mass = properties["properties"]["mass"];
            if (properties["properties"].contains("originX")) {
                this->originX = properties["properties"]["originX"];
            }
            if (properties["properties"].contains("originY")) {
                this->originY = properties["properties"]["originY"];
            }
            if (properties["properties"].contains("originZ")) {
                this->originZ = properties["properties"]["originZ"];
            }
            switch (bulletColliderResource::getBulletColliderTypeFromString(properties["properties"]["colliderType"])) {
                case BULLET_INVALID:
                    // Can't really do much here, we've already logged a warning about it
                    break;
                case BULLET_BOX:
                    this->collider = std::make_unique<btBoxShape>(btVector3(
                            properties["properties"]["boundX"],
                            properties["properties"]["boundY"],
                            properties["properties"]["boundZ"]));

                    this->transform.setIdentity();
                    this->transform.setOrigin(btVector3(this->originX, this->originY, this->originZ));

                    if (this->mass != 0.0f) {
                        this->collider->calculateLocalInertia(this->mass, this->localInertia);
                    }
                    break;
                case BULLET_BOX_2D: // move this case before BULLET_BOX
                case BULLET_BVH_TRIANGLE_MESH:
                case BULLET_CAPSULE_X:
                case BULLET_CAPSULE_Y:
                case BULLET_CAPSULE_Z:
                case BULLET_COMPOUND:
                case BULLET_CONE_X:
                case BULLET_CONE_Y:
                case BULLET_CONE_Z:
                case BULLET_CONVEX_2D:
                case BULLET_CONVEX_HULL:
                case BULLET_CONVEX_POINT_CLOUD:
                case BULLET_CONVEX_POLYHEDRON:
                case BULLET_CONVEX_TRIANGLE_MESH:
                case BULLET_CYLINDER_X:
                case BULLET_CYLINDER_Y:
                case BULLET_CYLINDER_Z:
                case BULLET_HEIGHTFIELD_TERRAIN:
                case BULLET_MULTIMATERIAL_TRIANGLE_MESH:
                case BULLET_MULTISPHERE:
                case BULLET_SCALED_BVH_TRIANGLE_MESH:
                case BULLET_SDF_COLLISION:
                case BULLET_SPHERE:
                case BULLET_STATIC_PLANE:
                case BULLET_TETRAHEDRON:
                case BULLET_UNIFORM_SCALING:
                    // todo: implement the rest of the collision shapes
                    break;
            }
        }
        btRigidBody* getNewRigidBody() {
            btRigidBody::btRigidBodyConstructionInfo info(
                    this->mass,
                    new btDefaultMotionState(this->transform),
                    this->collider.get(),
                    this->localInertia);
            return new btRigidBody(info);
        }
        static bulletColliderType getBulletColliderTypeFromString(const std::string& colliderTypeStr) {
            if (colliderTypeStr == "BULLET_BOX_2D") {
                return BULLET_BOX_2D;
            } else if (colliderTypeStr == "BULLET_BOX") {
                return BULLET_BOX;
            } else if (colliderTypeStr == "BULLET_BVH_TRIANGLE_MESH") {
                return BULLET_BVH_TRIANGLE_MESH;
            } else if (colliderTypeStr == "BULLET_CAPSULE_X") {
                return BULLET_CAPSULE_X;
            } else if (colliderTypeStr == "BULLET_CAPSULE_Y") {
                return BULLET_CAPSULE_Y;
            } else if (colliderTypeStr == "BULLET_CAPSULE_Z") {
                return BULLET_CAPSULE_Z;
            } else if (colliderTypeStr == "BULLET_COMPOUND") {
                return BULLET_COMPOUND;
            } else if (colliderTypeStr == "BULLET_CONE_X") {
                return BULLET_CONE_X;
            } else if (colliderTypeStr == "BULLET_CONE_Y") {
                return BULLET_CONE_Y;
            } else if (colliderTypeStr == "BULLET_CONE_Z") {
                return BULLET_CONE_Z;
            } else if (colliderTypeStr == "BULLET_CONVEX_2D") {
                return BULLET_CONVEX_2D;
            } else if (colliderTypeStr == "BULLET_CONVEX_HULL") {
                return BULLET_CONVEX_HULL;
            } else if (colliderTypeStr == "BULLET_CONVEX_POINT_CLOUD") {
                return BULLET_CONVEX_POINT_CLOUD;
            } else if (colliderTypeStr == "BULLET_CONVEX_POLYHEDRON") {
                return BULLET_CONVEX_POLYHEDRON;
            } else if (colliderTypeStr == "BULLET_CONVEX_TRIANGLE_MESH") {
                return BULLET_CONVEX_TRIANGLE_MESH;
            } else if (colliderTypeStr == "BULLET_CYLINDER_X") {
                return BULLET_CYLINDER_X;
            } else if (colliderTypeStr == "BULLET_CYLINDER_Y") {
                return BULLET_CYLINDER_Y;
            } else if (colliderTypeStr == "BULLET_CYLINDER_Z") {
                return BULLET_CYLINDER_Z;
            } else if (colliderTypeStr == "BULLET_HEIGHTFIELD_TERRAIN") {
                return BULLET_HEIGHTFIELD_TERRAIN;
            } else if (colliderTypeStr == "BULLET_MULTIMATERIAL_TRIANGLE_MESH") {
                return BULLET_MULTIMATERIAL_TRIANGLE_MESH;
            } else if (colliderTypeStr == "BULLET_MULTISPHERE") {
                return BULLET_MULTISPHERE;
            } else if (colliderTypeStr == "BULLET_SCALED_BVH_TRIANGLE_MESH") {
                return BULLET_SCALED_BVH_TRIANGLE_MESH;
            } else if (colliderTypeStr == "BULLET_SDF_COLLISION") {
                return BULLET_SDF_COLLISION;
            } else if (colliderTypeStr == "BULLET_SPHERE") {
                return BULLET_SPHERE;
            } else if (colliderTypeStr == "BULLET_STATIC_PLANE") {
                return BULLET_STATIC_PLANE;
            } else if (colliderTypeStr == "BULLET_TETRAHEDRON") {
                return BULLET_TETRAHEDRON;
            } else if (colliderTypeStr == "BULLET_UNIFORM_SCALING") {
                return BULLET_UNIFORM_SCALING;
            }
            logger::log(WARN, "BulletColliderResource", fmt::format(TR("warn.bullet_collider_resource.invalid_collider_type"), colliderTypeStr));
            return BULLET_INVALID;
        }
    private:
        std::unique_ptr<btCollisionShape> collider;
        btScalar mass = 0.0;
        btTransform transform;
        btVector3 localInertia;
        btScalar originX = 0.0;
        btScalar originY = 0.0;
        btScalar originZ = 0.0;
    };
}
