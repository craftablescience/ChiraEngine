#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <event/events.h>

using namespace chira;

TEST(events, broadcast) {
    const std::string broadcastName = "test_broadcast";
    events::broadcast(broadcastName);
    EXPECT_FALSE(events::hasBroadcast(broadcastName));
    events::clearBroadcasts();
    EXPECT_TRUE(events::hasBroadcast(broadcastName));
    events::clearBroadcasts();
}

TEST(events, singularEvent) {
    const std::string eventName = "test_event_singular";
    bool eventFired = false;
    auto id = events::addListener(eventName, [&eventFired](const std::any&) {eventFired = true;});
    EXPECT_FALSE(eventFired);
    events::runCallbacks();
    EXPECT_FALSE(eventFired);
    events::createEvent(eventName);
    events::runCallbacks();
    ASSERT_TRUE(eventFired);

    eventFired = false;
    events::removeListener(id);
    events::runCallbacks();
    EXPECT_FALSE(eventFired);
}

TEST(events, recursiveEvent) {
    const std::string eventName1 = "test_event_recursive_1";
    const std::string eventName2 = "test_event_recursive_2";
    int eventFired = 0;
    auto id1 = events::addListener(eventName1, [&eventFired, &eventName2](const std::any&) {
        eventFired += 1;
        events::createEvent(eventName2);
    });
    auto id2 = events::addListener(eventName2, [&eventFired](const std::any&) {eventFired += 1;});
    EXPECT_EQ(eventFired, 0);
    events::runCallbacks();
    EXPECT_EQ(eventFired, 0);
    events::createEvent(eventName1);
    events::runCallbacks();
    ASSERT_EQ(eventFired, 2);

    eventFired = 0;
    events::removeListener(id1);
    events::removeListener(id2);
    events::runCallbacks();
    EXPECT_EQ(eventFired, 0);
}

#pragma clang diagnostic pop
