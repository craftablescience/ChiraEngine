#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <core/config/ConEntry.h>

using namespace chira;

TEST(Logger, printConEntry) {
    LOG_BEGIN();

    ConCommand my_concommand{"my_concommand", "hi", [] {}};
    ASSERT_TRUE(ConEntryRegistry::hasConCommand("my_concommand"));

    ASSERT_TRUE(ConEntryRegistry::hasConCommand("con_entries"));
    ConEntryRegistry::getConCommand("con_entries")->fire({});

    ASSERT_TRUE(LOG_HAS_MESSAGE("my_concommand - hi"));

    LOG_END();
}

TEST(Logger, hiddenConEntry) {
    LOG_BEGIN();

    ConCommand my_hidden_concommand{"my_hidden_concommand", "hi", [] {}, CON_FLAG_HIDDEN};
    ASSERT_TRUE(ConEntryRegistry::hasConCommand("my_hidden_concommand"));

    ASSERT_TRUE(ConEntryRegistry::hasConCommand("con_entries"));
    ConEntryRegistry::getConCommand("con_entries")->fire({});

    ASSERT_FALSE(LOG_HAS_MESSAGE("my_hidden_concommand - hi"));

    LOG_END();
}
