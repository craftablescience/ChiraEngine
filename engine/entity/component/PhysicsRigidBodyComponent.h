#pragma once

#include <string>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <math/Types.h>
#include <plugin/Physics.h>
#include <render/mesh/MeshDataResource.h>
#include "TransformComponent.h"

namespace chira {

struct PhysicsRigidBodyComponent {
    PhysicsRigidBodyComponent()
            : mesh(Resource::getResource<MeshDataResource>("file://meshes/sphere.json")) {
        using namespace JPH::literals;

        // The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
        // variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
        JPH::BodyInterface& body_interface = Physics::get().GetBodyInterface();

        // Next we can create a rigid body to serve as the floor, we make a large box
        // Create the settings for the collision volume (the shape).
        // Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
        static JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));

        // Create the shape
        static JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
        static JPH::ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

        // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
        static JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(0.0_r, -1.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Physics::Layers::NON_MOVING);

        // Create the actual rigid body
        static JPH::Body* floor = body_interface.CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr

        // Add it to the world
        static auto x = [&body_interface] {
            body_interface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);
            return true;
        } ();

        // Now create a dynamic body to bounce on the floor
        // Note that this uses the shorthand version of creating and adding a body to the world
        JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(1.f), JPH::RVec3(static_cast<float>(std::rand()) / RAND_MAX, 20.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Physics::Layers::MOVING);
        this->sphere = body_interface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);

        // Now you can interact with the dynamic body, in this case we're going to give it a velocity.
        // (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
        body_interface.SetLinearVelocity(this->sphere, JPH::Vec3(0.0f, -5.0f, 0.0f));
    }

public:
    TransformComponent* transform = nullptr;
    SharedPointer<MeshDataResource> mesh;
    JPH::BodyID sphere;
};

} // namespace chira
