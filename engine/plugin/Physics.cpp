#include "Physics.h"

#include <memory>
#include <thread>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <core/Assertions.h>
#include <core/Engine.h>
#include "Plugin.h"

using namespace chira;

constexpr int JOLT_MAX_RIGIDBODIES = 65536;
constexpr int JOLT_MAX_PHYSICS_JOBS = 256;
constexpr int JOLT_MAX_PHYSICS_BARRIERS = 256;

// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
constexpr int JOLT_MAX_BODY_PAIRS = 65536;

// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
constexpr int JOLT_MAX_CONTACT_CONSTRAINTS = 10240;

constexpr int JOLT_PHYSICS_SUBSTEPS = 1;

#ifdef JPH_ENABLE_ASSERTS
static bool assertFailedImpl(const char* /*inExpression*/, const char* inMessage, const char* inFile, unsigned int inLine) {
    runtime_assert_internal(false, inMessage, inFile, inLine, __FUNCTION__);
    return true;
};
#endif

namespace BroadPhaseLayers {

static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
static constexpr JPH::BroadPhaseLayer MOVING(1);
static constexpr unsigned int NUM_LAYERS(2);

} // namespace BroadPhaseLayers

/// Class that determines if two object layers can collide
class ChiraObjectLayerPairFilter : public JPH::ObjectLayerPairFilter {
public:
    [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override {
        switch (inObject1) {
            case Physics::Layers::NON_MOVING:
                return inObject2 == Physics::Layers::MOVING; // Non-moving only collides with moving
            case Physics::Layers::MOVING:
                return true; // Moving collides with everything
            default:
                //JPH_ASSERT(false);
                return false;
        }
    }
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class ChiraBPLayerInterface final : public JPH::BroadPhaseLayerInterface {
public:
    ChiraBPLayerInterface() = default;

    [[nodiscard]] unsigned int GetNumBroadPhaseLayers() const override {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
        //JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    [[nodiscard]] const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override {
        switch ((JPH::BroadPhaseLayer::Type) inLayer) {
            case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
                return "NON_MOVING";
            case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
                return "MOVING";
            default:
                // JPH_ASSERT(false);
                return "INVALID";
        }
    }
#endif

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Physics::Layers::NUM_LAYERS] {
            BroadPhaseLayers::NON_MOVING,
            BroadPhaseLayers::MOVING,
    };
};

/// Class that determines if an object layer can collide with a broadphase layer
class ChiraObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
        switch (inLayer1) {
            case Physics::Layers::NON_MOVING:
                return inLayer2 == BroadPhaseLayers::MOVING;
            case Physics::Layers::MOVING:
                return true;
            default:
                return false;
        }
    }
};

/*
// An example contact listener
class MyContactListener : public JPH::ContactListener {
public:
    // See: ContactListener
    JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override {
        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override {
        // contact added
    }

    void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override {
        // contact persisted
    }

    void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override {
        // contact removed
    }
};

// An example activation listener
class MyBodyActivationListener : public JPH::BodyActivationListener {
public:
    void OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override {
        // body activated
    }

    void OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override {
        // body sleeping
    }
};
*/

std::unique_ptr<JPH::TempAllocatorMalloc> g_JoltTempAllocator;
std::unique_ptr<JPH::JobSystemThreadPool> g_JoltJobSystem;
std::unique_ptr<ChiraBPLayerInterface> g_JoltBroadPhaseLayerInterface;
std::unique_ptr<ChiraObjectVsBroadPhaseLayerFilter> g_JoltObjectVsBroadPhaseLayerFilter;
std::unique_ptr<ChiraObjectLayerPairFilter> g_JoltObjectVsObjectLayerFilter;
std::unique_ptr<JPH::PhysicsSystem> g_JoltPhysicsSystem;

struct PhysicsPlugin final : public IPlugin {
    void init() override {
        JPH::RegisterDefaultAllocator();

        //JPH::Trace = traceImpl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = assertFailedImpl;)

        // Create a factory
        JPH::Factory::sInstance = new JPH::Factory();

        JPH::RegisterTypes();

        g_JoltTempAllocator = std::make_unique<JPH::TempAllocatorMalloc>();

        g_JoltJobSystem = std::make_unique<JPH::JobSystemThreadPool>(JOLT_MAX_PHYSICS_JOBS, JOLT_MAX_PHYSICS_BARRIERS, static_cast<int>(std::thread::hardware_concurrency()) - 1);

        g_JoltBroadPhaseLayerInterface = std::make_unique<ChiraBPLayerInterface>();
        g_JoltObjectVsBroadPhaseLayerFilter = std::make_unique<ChiraObjectVsBroadPhaseLayerFilter>();
        g_JoltObjectVsObjectLayerFilter = std::make_unique<ChiraObjectLayerPairFilter>();

        g_JoltPhysicsSystem = std::make_unique<JPH::PhysicsSystem>();
        g_JoltPhysicsSystem->Init(JOLT_MAX_RIGIDBODIES, 0, JOLT_MAX_BODY_PAIRS, JOLT_MAX_CONTACT_CONSTRAINTS, *g_JoltBroadPhaseLayerInterface, *g_JoltObjectVsBroadPhaseLayerFilter, *g_JoltObjectVsObjectLayerFilter);

        // A body activation listener gets notified when bodies activate and go to sleep
        // Note that this is called from a job so whatever you do here needs to be thread safe!
        //MyBodyActivationListener body_activation_listener;
        //g_JoltPhysicsSystem->SetBodyActivationListener(&body_activation_listener);

        // A contact listener gets notified when bodies (are about to) collide, and when they separate again.
        // Note that this is called from a job so whatever you do here needs to be thread safe!
        //MyContactListener contact_listener;
        //g_JoltPhysicsSystem->SetContactListener(&contact_listener);
    }
    void update() override {
        g_JoltPhysicsSystem->Update(static_cast<float>(Engine::getDeltaTicks()) / 1000.f, 1, JOLT_PHYSICS_SUBSTEPS, g_JoltTempAllocator.get(), g_JoltJobSystem.get());
    }
    void deinit() override {
        JPH::UnregisterTypes();

        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }
};
CHIRA_REGISTER_PLUGIN(PhysicsPlugin);

JPH::PhysicsSystem& Physics::get() {
    return *g_JoltPhysicsSystem;
}
