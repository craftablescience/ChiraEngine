#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <utility/memory/sharedPointer.h>

using namespace chira;

struct SharedPointerTestBase {
    int x = 10;
    virtual ~SharedPointerTestBase() = default;
};

struct SharedPointerTestDerived : public SharedPointerTestBase {
    int y = -10;
};

TEST(sharedPointer, createFromPointer) {
    auto a = SharedPointer<SharedPointerTestBase>(new SharedPointerTestBase{});
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
}

TEST(sharedPointer, createFromSharedCopyConstructor) {
    auto a = SharedPointer<SharedPointerTestBase>(new SharedPointerTestBase{});
    a.setHolderAmountForDelete(0);
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
    {
        auto b = SharedPointer<SharedPointerTestBase>{a};
        EXPECT_EQ(a.useCount(), 2);
        EXPECT_EQ(b.useCount(), 2);
        EXPECT_EQ(a.get(), b.get());
    }
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ((*a).x, 10);
}

TEST(sharedPointer, createFromSharedCopyAssignment) {
    auto a = SharedPointer<SharedPointerTestBase>(new SharedPointerTestBase{});
    a.setHolderAmountForDelete(0);
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

TEST(sharedPointer, createFromCastDynamic) {
    auto a = SharedPointer<SharedPointerTestDerived>(new SharedPointerTestDerived{});
    a.setHolderAmountForDelete(0);
    EXPECT_EQ(a.useCount(), 1);
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ(a->y, -10);
    EXPECT_EQ((*a).x, 10);
    EXPECT_EQ((*a).y, -10);
    {
        auto b = a.castDynamic<SharedPointerTestBase>();
        EXPECT_EQ(a.useCount(), 2);
        EXPECT_EQ(b.useCount(), 2);
        EXPECT_EQ(a.get(), b.get());
        EXPECT_EQ(b->x, 10);
        b->x += 10;

        auto c = b.castDynamic<SharedPointerTestDerived>();
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
