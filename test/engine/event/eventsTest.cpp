#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <event/events.h>

using namespace chira;

TEST(events, broadcast) {
    const std::string broadcastName = "test_broadcast";
    Events::broadcast(broadcastName);
    EXPECT_FALSE(Events::hasBroadcast(broadcastName));
    Events::clearBroadcasts();
    EXPECT_TRUE(Events::hasBroadcast(broadcastName));
    Events::clearBroadcasts();
}

TEST(events, singularEvent) {
    const std::string eventName = "test_event_singular";
    bool eventFired = false;
    auto id = Events::addListener(eventName, [&eventFired](const std::any&) {eventFired = true;});
    EXPECT_FALSE(eventFired);
    Events::runCallbacks();
    EXPECT_FALSE(eventFired);
    Events::createEvent(eventName);
    Events::runCallbacks();
    ASSERT_TRUE(eventFired);

    eventFired = false;
    Events::removeListener(id);
    Events::runCallbacks();
    EXPECT_FALSE(eventFired);
}

TEST(events, recursiveEvent) {
    const std::string eventName1 = "test_event_recursive_1";
    const std::string eventName2 = "test_event_recursive_2";
    int eventFired = 0;
    auto id1 = Events::addListener(eventName1, [&eventFired, &eventName2](const std::any&) {
        eventFired += 1;
        Events::createEvent(eventName2);
    });
    auto id2 = Events::addListener(eventName2, [&eventFired](const std::any&) {eventFired += 1;});
    EXPECT_EQ(eventFired, 0);
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 0);
    Events::createEvent(eventName1);
    Events::runCallbacks();
    ASSERT_EQ(eventFired, 2);

    eventFired = 0;
    Events::removeListener(id1);
    Events::removeListener(id2);
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 0);
}

TEST(events, multipleFiringsOfOneEvent) {
    const std::string eventName = "test_event_multiple_firings";
    int eventFired = 0;
    auto id = Events::addListener(eventName, [&eventFired](const std::any& input) {
        try { eventFired += std::any_cast<int>(input); }
        catch (std::bad_any_cast&) { eventFired -= 1; }
    });

    EXPECT_EQ(eventFired, 0);
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 0);

    Events::createEvent(eventName, 2);
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 2);

    Events::createEvent(eventName, 3);
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 5);

    Events::createEvent(eventName, "throws std::bad_any_cast");
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 4);

    eventFired = 0;
    Events::createEvent(eventName, 1);
    Events::createEvent(eventName, 2);
    Events::createEvent(eventName, 3);
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 6);

    eventFired = 0;
    Events::removeListener(id);
    Events::runCallbacks();
    EXPECT_EQ(eventFired, 0);
}

#pragma clang diagnostic pop
