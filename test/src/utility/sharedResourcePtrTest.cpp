#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <utility/sharedPointer.h>

using namespace chira;

struct SharedResourcePtrTestBase {
    int x = 10;
    virtual ~SharedResourcePtrTestBase() = default;
};

struct SharedResourcePtrTestDerived : public SharedResourcePtrTestBase {
    int y = -10;
};

TEST(sharedResourcePtr, createFromPointer) {
    auto a = sharedPointer<SharedResourcePtrTestBase>(new SharedResourcePtrTestBase{});
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
}

TEST(sharedResourcePtr, createFromSharedCopyConstructor) {
    auto a = sharedPointer<SharedResourcePtrTestBase>(new SharedResourcePtrTestBase{});
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
    {
        auto b = sharedPointer<SharedResourcePtrTestBase>{a};
        EXPECT_EQ(a.useCount(), 2);
        EXPECT_EQ(b.useCount(), 2);
        EXPECT_EQ(a.get(), b.get());
    }
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
}

TEST(sharedResourcePtr, createFromSharedCopyAssignment) {
    auto a = sharedPointer<SharedResourcePtrTestBase>(new SharedResourcePtrTestBase{});
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
    {
        auto b = a;
        EXPECT_EQ(a.useCount(), 2);
        EXPECT_EQ(b.useCount(), 2);
        EXPECT_EQ(a.get(), b.get());
    }
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
}

TEST(sharedResourcePtr, createFromCastDynamic) {
    auto a = sharedPointer<SharedResourcePtrTestDerived>(new SharedResourcePtrTestDerived{});
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ(a->y, -10);
    EXPECT_EQ((*a).x, 10);
    EXPECT_EQ((*a).y, -10);
    {
        auto b = a.castDynamic<SharedResourcePtrTestBase>();
        EXPECT_EQ(a.useCount(), 2);
        EXPECT_EQ(b.useCount(), 2);
        EXPECT_EQ(a.get(), b.get());
        EXPECT_EQ(b->x, 10);
        b->x += 10;

        auto c = b.castDynamic<SharedResourcePtrTestDerived>();
        EXPECT_EQ(a.useCount(), 3);
        EXPECT_EQ(b.useCount(), 3);
        EXPECT_EQ(c.useCount(), 3);
        EXPECT_EQ(b.get(), c.get());
        EXPECT_EQ(c->x, 20);
        EXPECT_EQ(c->y, -10);
    }
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 20);
    EXPECT_EQ((*a).x, 20);
}

#pragma clang diagnostic pop
