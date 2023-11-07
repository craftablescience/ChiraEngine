#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <filesystem>

#include <core/config/ConEntry.h>
#include <core/config/Config.h>

using namespace chira;

TEST(ConCommand, createConCommand) {
    EXPECT_FALSE(ConEntryRegistry::hasConCommand("my_concommand"));
    {
        ConCommand my_concommand{"my_concommand", [] {}};
        EXPECT_TRUE(ConEntryRegistry::hasConCommand("my_concommand"));
    }
    EXPECT_FALSE(ConEntryRegistry::hasConCommand("my_concommand"));
}

TEST(ConCommand, callbackCallsBack) {
    bool check = false;
    ConCommand my_concommand{"my_concommand", [&check] {
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
    ConCommand my_cheat_concommand{"my_cheat_concommand", [&check] {
        check = true;
    }, CON_FLAG_CHEAT};

    ConEntryRegistry::getConVar("sv_cheats")->setValue(false);
    my_cheat_concommand.fire({});
    ASSERT_FALSE(check);

    ConEntryRegistry::getConVar("sv_cheats")->setValue(true);
    my_cheat_concommand.fire({});
    ASSERT_TRUE(check);

    // Put it back the way you found it!
    ConEntryRegistry::getConVar("sv_cheats")->setValue(false);
}

TEST(ConVar, createConVar) {
    ConVar my_convar{"my_convar", false};
    ASSERT_FALSE(my_convar.getValue<bool>());

    my_convar.setValue(true);
    ASSERT_TRUE(my_convar.getValue<bool>());
}

TEST(ConVar, useConVarSetValueCallback) {
    bool check_bool = false, check_int = false, check_double = false, check_string = false;

    ConVar my_convar_bool{"my_convar_bool", true, CON_FLAG_NONE, [&check_bool](ConVar::CallbackArg) {
        check_bool = true;
    }};
    my_convar_bool.setValue(false);
    EXPECT_FALSE(my_convar_bool.getValue<bool>());
    EXPECT_TRUE(check_bool);

    ConVar my_convar_int{"my_convar_int", 2, CON_FLAG_NONE, [&check_int](ConVar::CallbackArg) {
        check_int = true;
    }};
    my_convar_int.setValue(3);
    EXPECT_EQ(my_convar_int.getValue<int>(), 3);
    EXPECT_TRUE(check_int);

    ConVar my_convar_double{"my_convar_double", 4.5, CON_FLAG_NONE, [&check_double](ConVar::CallbackArg) {
        check_double = true;
    }};
    my_convar_double.setValue(2.5);
    EXPECT_FLOAT_EQ(my_convar_double.getValue<double>(), 2.5);
    EXPECT_TRUE(check_double);

    ConVar my_convar_string{"my_convar_string", std::string{"hello"}, CON_FLAG_NONE, [&check_string](ConVar::CallbackArg) {
        check_string = true;
    }};
    my_convar_string.setValue(std::string{"world"});
    EXPECT_STREQ(my_convar_string.getValue<std::string>().c_str(), "world");
    EXPECT_TRUE(check_string);
}

TEST(ConVar, typesConstructedCorrectly) {
    ConVar my_convar_bool{"my_convar_bool", true};
    EXPECT_EQ(my_convar_bool.getType(), ConVarType::BOOLEAN);

    ConVar my_convar_int{"my_convar_int", 0};
    EXPECT_EQ(my_convar_int.getType(), ConVarType::INTEGER);

    ConVar my_convar_double{"my_convar_double", 0.0};
    EXPECT_EQ(my_convar_double.getType(), ConVarType::DOUBLE);

    ConVar my_convar_string{"my_convar_string", std::string{}};
    EXPECT_EQ(my_convar_string.getType(), ConVarType::STRING);
}

TEST(ConVar, typeConversionsGetter) {
    ConVar my_convar_bool{"my_convar_bool", false};
    EXPECT_FALSE(my_convar_bool.getValue<bool>());
    EXPECT_EQ(my_convar_bool.getValue<int>(), 0);
    EXPECT_FLOAT_EQ(my_convar_bool.getValue<double>(), 0.0);
    EXPECT_STREQ(my_convar_bool.getValue<std::string>().c_str(), "0");

    ConVar my_convar_int{"my_convar_int", 2};
    EXPECT_TRUE(my_convar_int.getValue<bool>());
    EXPECT_EQ(my_convar_int.getValue<int>(), 2);
    EXPECT_FLOAT_EQ(my_convar_int.getValue<double>(), 2.0);
    EXPECT_STREQ(my_convar_int.getValue<std::string>().c_str(), "2");

    ConVar my_convar_double{"my_convar_double", 1.5};
    EXPECT_TRUE(my_convar_double.getValue<bool>());
    EXPECT_EQ(my_convar_double.getValue<int>(), 1);
    EXPECT_FLOAT_EQ(my_convar_double.getValue<double>(), 1.5);
    EXPECT_STREQ(my_convar_double.getValue<std::string>().substr(0,3).c_str(), "1.5");

    // Getters return size of string
    ConVar my_convar_string{"my_convar_string", std::string{"hi"}};
    EXPECT_TRUE(my_convar_string.getValue<bool>());
    EXPECT_EQ(my_convar_string.getValue<int>(), 2);
    EXPECT_FLOAT_EQ(my_convar_string.getValue<double>(), 2.0);
    EXPECT_STREQ(my_convar_string.getValue<std::string>().c_str(), "hi");
}

TEST(ConVar, typeConversionsSetter) {
    ConVar my_convar_bool{"my_convar_bool", false};
    my_convar_bool.setValue(15);
    EXPECT_TRUE(my_convar_bool.getValue<bool>());
    my_convar_bool.setValue(0.0);
    EXPECT_FALSE(my_convar_bool.getValue<bool>());
    my_convar_bool.setValue(std::string{"hello"});
    EXPECT_TRUE(my_convar_bool.getValue<bool>());

    ConVar my_convar_int{"my_convar_int", 2};
    my_convar_int.setValue(false);
    EXPECT_EQ(my_convar_int.getValue<int>(), 0);
    my_convar_int.setValue(10.0);
    EXPECT_EQ(my_convar_int.getValue<int>(), 10);
    my_convar_int.setValue(std::string{"hello"});
    EXPECT_EQ(my_convar_int.getValue<int>(), 5);

    ConVar my_convar_double{"my_convar_float", 1.5};
    my_convar_double.setValue(false);
    EXPECT_FLOAT_EQ(my_convar_double.getValue<double>(), 0.0);
    my_convar_double.setValue(4);
    EXPECT_FLOAT_EQ(my_convar_double.getValue<double>(), 4.0);
    my_convar_double.setValue(std::string{"hello"});
    EXPECT_FLOAT_EQ(my_convar_double.getValue<double>(), 5.0);

    ConVar my_convar_string{"my_convar_string", std::string{"hello"}};
    my_convar_string.setValue(true);
    EXPECT_STREQ(my_convar_string.getValue<std::string>().c_str(), "1");
    my_convar_string.setValue(23);
    EXPECT_STREQ(my_convar_string.getValue<std::string>().c_str(), "23");
    my_convar_string.setValue(34.5);
    EXPECT_STREQ(my_convar_string.getValue<std::string>().substr(0,4).c_str(), "34.5");
}

TEST(ConVar, cheatConVar) {
    ConVar my_cheat_convar{"my_cheat_convar", false, CON_FLAG_CHEAT};

    ConEntryRegistry::getConVar("sv_cheats")->setValue(false);
    my_cheat_convar.setValue(true);
    ASSERT_FALSE(my_cheat_convar.getValue<bool>());

    ConEntryRegistry::getConVar("sv_cheats")->setValue(true);
    my_cheat_convar.setValue(true);
    ASSERT_TRUE(my_cheat_convar.getValue<bool>());

    // Put it back the way you found it!
    ConEntryRegistry::getConVar("sv_cheats")->setValue(false);
}

TEST(ConEntryRegistry, hasConVar) {
    EXPECT_FALSE(ConEntryRegistry::hasConVar("my_convar"));

    {
        ConVar my_convar{"my_convar", 0};
        EXPECT_TRUE(ConEntryRegistry::hasConVar("my_convar"));
    }

    EXPECT_FALSE(ConEntryRegistry::hasConVar("my_convar"));
}

TEST(ConEntryRegistry, cacheConVar) {
    // Just in case!
    std::filesystem::remove(Config::getConfigFile("convars.json"));
    {
        JSONConfigFile cache{"convars.json"};
        EXPECT_FALSE(cache.hasValue("my_cached_convar_bool"));
        EXPECT_FALSE(cache.hasValue("my_cached_convar_int"));
        EXPECT_FALSE(cache.hasValue("my_cached_convar_double"));
        EXPECT_FALSE(cache.hasValue("my_cached_convar_string"));
    }
    {
        ConVar my_cached_convar_bool{"my_cached_convar_bool", true, CON_FLAG_CACHE};
        ConVar my_cached_convar_int{"my_cached_convar_int", 14, CON_FLAG_CACHE};
        ConVar my_cached_convar_double{"my_cached_convar_double", 5.5, CON_FLAG_CACHE};
        ConVar my_cached_convar_string{"my_cached_convar_string", std::string{"hello"}, CON_FLAG_CACHE};
    }
    {
        JSONConfigFile cache{"convars.json"};
        EXPECT_TRUE(cache.hasValue("my_cached_convar_bool"));
        EXPECT_TRUE(cache.hasValue("my_cached_convar_int"));
        EXPECT_TRUE(cache.hasValue("my_cached_convar_double"));
        EXPECT_TRUE(cache.hasValue("my_cached_convar_string"));
    }
    {
        // Use different defaults! Should be the same as setting them in the console to a different value
        ConVar my_cached_convar_bool{"my_cached_convar_bool", false, CON_FLAG_CACHE};
        ConVar my_cached_convar_int{"my_cached_convar_int", 8, CON_FLAG_CACHE};
        ConVar my_cached_convar_double{"my_cached_convar_double", 13.25, CON_FLAG_CACHE};
        ConVar my_cached_convar_string{"my_cached_convar_string", std::string{"world"}, CON_FLAG_CACHE};
        EXPECT_TRUE(my_cached_convar_bool.getValue<bool>());
        EXPECT_EQ(my_cached_convar_int.getValue<int>(), 14);
        EXPECT_FLOAT_EQ(my_cached_convar_double.getValue<double>(), 5.5);
        EXPECT_STREQ(my_cached_convar_string.getValue<std::string>().c_str(), "hello");
    }
    // Clean up after ourselves
    std::filesystem::remove(Config::getConfigFile("convars.json"));
}

TEST(ConCommandRef, all) {
    int x = 0;
    ConCommand test{"test", [&x] (ConCommand::CallbackArgs) { x++; }};
    {
        ConCommandRef testRef{"test"};
        ASSERT_TRUE(testRef);
        testRef.fire({});
        EXPECT_EQ(x, 1);
    }
    {
        ConCommandRef testRef{"nonexistent"};
        EXPECT_FALSE(testRef);
    }
}

TEST(ConVarRef, all) {
    ConVar test{"test", 0};
    {
        ConVarRef testRef{"test"};
        ASSERT_TRUE(testRef);
        EXPECT_EQ(testRef.getValue<int>(), 0);
        testRef.setValue(1);
        EXPECT_EQ(testRef.getValue<int>(), 1);
    }
    EXPECT_EQ(test.getValue<int>(), 1);
    {
        ConVarRef testRef{"nonexistent"};
        EXPECT_FALSE(testRef);
    }
}
