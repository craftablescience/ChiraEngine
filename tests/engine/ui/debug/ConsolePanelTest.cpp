#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <config/ConEntry.h>

using namespace chira;

TEST(ConsolePanel, printConEntry) {
    LOG_BEGIN();

    ConCommand my_concommand{"my_concommand", "hi", [] {}};
    ASSERT_TRUE(ConCommandRegistry::hasConCommand("my_concommand"));

    ASSERT_TRUE(ConCommandRegistry::hasConCommand("con_entries"));
    ConCommandRegistry::getConCommand("con_entries")->fire({});

    ASSERT_TRUE(LOG_HAS_MESSAGE("my_concommand - hi"));

    LOG_END();
}

TEST(ConsolePanel, hiddenConEntry) {
    LOG_BEGIN();

    ConCommand my_hidden_concommand{"my_hidden_concommand", "hi", [] {}, CON_FLAG_HIDDEN};
    ASSERT_TRUE(ConCommandRegistry::hasConCommand("my_hidden_concommand"));

    ASSERT_TRUE(ConCommandRegistry::hasConCommand("con_entries"));
    ConCommandRegistry::getConCommand("con_entries")->fire({});

    ASSERT_FALSE(LOG_HAS_MESSAGE("my_hidden_concommand - hi"));

    LOG_END();
}
