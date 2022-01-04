#include "bulletColliderResource.h"

#include <utility/logger.h>
#include <utility/math/axis.h>
#include <i18n/translationManager.h>

using namespace chira;

void BulletColliderResource::compile(const nlohmann::json& properties) {
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
    this->colliderType = BulletColliderResource::getBulletColliderTypeFromString(properties["properties"]["colliderType"]);
    switch(this->colliderType) {
        case BulletColliderType::BULLET_INVALID:
            // Can't really do much here, we've already logged a warning about it
            break;

        case BulletColliderType::BULLET_BOX:
            this->collider = std::make_unique<btBoxShape>(btVector3(
                    properties["properties"]["boundX"],
                    properties["properties"]["boundY"],
                    properties["properties"]["boundZ"]));
            break;

        case BulletColliderType::BULLET_BVH_TRIANGLE_MESH:
            // todo
            Logger::log(LogType::ERROR, "BulletColliderResource", fmt::format(TR("error.bullet_collider_resource.unimplemented"), properties["properties"]["colliderType"]));
            break;

        case BulletColliderType::BULLET_CAPSULE:
            switch (getAxisFromString(properties["properties"]["axis"])) {
                case Axis::X:
                    this->collider = std::make_unique<btCapsuleShapeX>(
                            properties["properties"]["radius"],
                            properties["properties"]["height"]);
                    break;
                case Axis::Y:
                    this->collider = std::make_unique<btCapsuleShape>(
                            properties["properties"]["radius"],
                            properties["properties"]["height"]);
                    break;
                case Axis::Z:
                    this->collider = std::make_unique<btCapsuleShapeZ>(
                            properties["properties"]["radius"],
                            properties["properties"]["height"]);
                    break;
                default:
                    // Just don't make the collider, a warning has already been sent
                    break;
            }
            break;

        case BulletColliderType::BULLET_COMPOUND:
            // todo
            Logger::log(LogType::ERROR, "BulletColliderResource", fmt::format(TR("error.bullet_collider_resource.unimplemented"), properties["properties"]["colliderType"]));
            break;

        case BulletColliderType::BULLET_CONE:
            switch (getAxisFromString(properties["properties"]["axis"])) {
                case Axis::X:
                    this->collider = std::make_unique<btConeShapeX>(
                            properties["properties"]["radius"],
                            properties["properties"]["height"]);
                    break;
                case Axis::Y:
                    this->collider = std::make_unique<btConeShape>(
                            properties["properties"]["radius"],
                            properties["properties"]["height"]);
                    break;
                case Axis::Z:
                    this->collider = std::make_unique<btConeShapeZ>(
                            properties["properties"]["radius"],
                            properties["properties"]["height"]);
                    break;
                default:
                    // Just don't make the collider, a warning has already been sent
                    break;
            }
            break;

        case BulletColliderType::BULLET_CONVEX_HULL: // todo
        case BulletColliderType::BULLET_CONVEX_POINT_CLOUD: // todo
        case BulletColliderType::BULLET_CONVEX_POLYHEDRON: // todo
        case BulletColliderType::BULLET_CONVEX_TRIANGLE_MESH: // todo
            Logger::log(LogType::ERROR, "BulletColliderResource", fmt::format(TR("error.bullet_collider_resource.unimplemented"), properties["properties"]["colliderType"]));
            break;

        case BulletColliderType::BULLET_CYLINDER:
            switch (getAxisFromString(properties["properties"]["axis"])) {
                case Axis::X:
                    this->collider = std::make_unique<btCylinderShapeX>(btVector3(
                            properties["properties"]["height"],
                            properties["properties"]["radius"],
                            1)); // Z-value not used
                    break;
                case Axis::Y:
                    this->collider = std::make_unique<btCylinderShape>(btVector3(
                            properties["properties"]["radius"],
                            properties["properties"]["height"],
                            1)); // Z-value not used
                    break;
                case Axis::Z:
                    this->collider = std::make_unique<btCylinderShapeZ>(btVector3(
                            properties["properties"]["radius"],
                            1,
                            properties["properties"]["height"])); // Y-value not used
                    break;
                default:
                    // Just don't make the collider, a warning has already been sent
                    break;
            }
            break;

        case BulletColliderType::BULLET_HEIGHTFIELD_TERRAIN: // todo
        case BulletColliderType::BULLET_MULTIMATERIAL_TRIANGLE_MESH: // todo
        case BulletColliderType::BULLET_MULTISPHERE: // todo
        case BulletColliderType::BULLET_SDF_COLLISION: // todo
            Logger::log(LogType::ERROR, "BulletColliderResource", fmt::format(TR("error.bullet_collider_resource.unimplemented"), properties["properties"]["colliderType"]));
            break;

        case BulletColliderType::BULLET_SPHERE:
            this->collider = std::make_unique<btSphereShape>(properties["properties"]["radius"]);
            break;

        case BulletColliderType::BULLET_STATIC_PLANE:
            this->collider = std::make_unique<btStaticPlaneShape>(btVector3(
                    properties["properties"]["normalX"],
                    properties["properties"]["normalY"],
                    properties["properties"]["normalZ"]),
                       properties["properties"]["planeConstant"]);
            break;
    }

    this->transform.setIdentity();
    this->transform.setOrigin(btVector3(this->originX, this->originY, this->originZ));

    if (this->colliderType == BulletColliderType::BULLET_STATIC_PLANE) {
        this->mass = 0.0f;
    }
    if (this->mass != 0.0f) {
        this->collider->calculateLocalInertia(this->mass, this->localInertia);
    }
}

btRigidBody* BulletColliderResource::getNewRigidBody() const {
    btRigidBody::btRigidBodyConstructionInfo info(
            this->mass,
            new btDefaultMotionState(this->transform),
            this->collider.get(),
            this->localInertia);
    return new btRigidBody(info);
}

BulletColliderType BulletColliderResource::getBulletColliderTypeFromString(const std::string& colliderTypeStr) {
    if (colliderTypeStr == "BULLET_BOX")
        return BulletColliderType::BULLET_BOX;
    else if (colliderTypeStr == "BULLET_BVH_TRIANGLE_MESH")
        return BulletColliderType::BULLET_BVH_TRIANGLE_MESH;
    else if (colliderTypeStr == "BULLET_CAPSULE_X")
        return BulletColliderType::BULLET_CAPSULE;
    else if (colliderTypeStr == "BULLET_COMPOUND")
        return BulletColliderType::BULLET_COMPOUND;
    else if (colliderTypeStr == "BULLET_CONE_X")
        return BulletColliderType::BULLET_CONE;
    else if (colliderTypeStr == "BULLET_CONVEX_HULL")
        return BulletColliderType::BULLET_CONVEX_HULL;
    else if (colliderTypeStr == "BULLET_CONVEX_POINT_CLOUD")
        return BulletColliderType::BULLET_CONVEX_POINT_CLOUD;
    else if (colliderTypeStr == "BULLET_CONVEX_POLYHEDRON")
        return BulletColliderType::BULLET_CONVEX_POLYHEDRON;
    else if (colliderTypeStr == "BULLET_CONVEX_TRIANGLE_MESH")
        return BulletColliderType::BULLET_CONVEX_TRIANGLE_MESH;
    else if (colliderTypeStr == "BULLET_CYLINDER_X")
        return BulletColliderType::BULLET_CYLINDER;
    else if (colliderTypeStr == "BULLET_HEIGHTFIELD_TERRAIN")
        return BulletColliderType::BULLET_HEIGHTFIELD_TERRAIN;
    else if (colliderTypeStr == "BULLET_MULTIMATERIAL_TRIANGLE_MESH")
        return BulletColliderType::BULLET_MULTIMATERIAL_TRIANGLE_MESH;
    else if (colliderTypeStr == "BULLET_MULTISPHERE")
        return BulletColliderType::BULLET_MULTISPHERE;
    else if (colliderTypeStr == "BULLET_SDF_COLLISION")
        return BulletColliderType::BULLET_SDF_COLLISION;
    else if (colliderTypeStr == "BULLET_SPHERE")
        return BulletColliderType::BULLET_SPHERE;
    else if (colliderTypeStr == "BULLET_STATIC_PLANE")
        return BulletColliderType::BULLET_STATIC_PLANE;

    Logger::log(LogType::WARNING, "BulletColliderResource", fmt::format(TR("warn.bullet_collider_resource.invalid_collider_type"), colliderTypeStr));
    return BulletColliderType::BULLET_INVALID;
}
