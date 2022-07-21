#include <gtest/gtest.h>

#include <utility/ConCommand.h>
#include <utility/ConVar.h>

using namespace chira;

TEST(ConCommand, createConCommand) {
    {
        ConCommand my_concommand{"my_concommand", [](const std::vector<std::string> &) {}};
        ASSERT_TRUE(ConCommandRegistry::hasConCommand("my_concommand"));
    }
    ASSERT_FALSE(ConCommandRegistry::hasConCommand("my_concommand"));
}

TEST(ConCommand, callbackCallsBack) {
    bool check = false;
    ConCommand my_concommand{"my_concommand", [&check](const std::vector<std::string>&) {
        check = true;
    }};
    my_concommand.fire({});
    ASSERT_TRUE(check);
}

TEST(ConCommand, callbackGetsArgs) {
    std::string check1, check2;
    ConCommand my_concommand{"my_concommand", [&check1, &check2](const std::vector<std::string>& args) {
        ASSERT_EQ(args.size(), 2);
        check1 = args[0];
        check2 = args[1];
    }};
    my_concommand.fire({"hello", "world"});
    ASSERT_STREQ(check1.c_str(), "hello");
    ASSERT_STREQ(check2.c_str(), "world");
}

TEST(ConCommand, cheatConCommand) {
    bool check = false;
    ConCommand my_cheat_concommand{"my_cheat_concommand", [&check](const std::vector<std::string>&) {
        check = true;
    }, CONVAR_FLAG_CHEAT};
    ASSERT_FALSE(areCheatsEnabled());

    my_cheat_concommand.fire({});
    ASSERT_FALSE(check);

    ConVarReference cheats{"cheats"};
    cheats->setValue(true);
    my_cheat_concommand.fire({});
    ASSERT_TRUE(check);
}
