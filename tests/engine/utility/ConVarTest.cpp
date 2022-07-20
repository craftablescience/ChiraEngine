#include <gtest/gtest.h>

#include <utility/ConVar.h>

using namespace chira;

TEST(ConVar, createConVar) {
    ConVar my_convar{"my_convar", false};
    ASSERT_FALSE(my_convar.getValue<bool>());

    my_convar.setValue(true);
    ASSERT_TRUE(my_convar.getValue<bool>());
}

TEST(ConVar, useConVarSetValueCallback) {
    bool check_bool = false, check_int = false, check_float = false;

    ConVar my_convar_bool{"my_convar_bool", true, "", [&check_bool](bool newValue) {
        check_bool = true;
    }};
    my_convar_bool.setValue(false);
    EXPECT_FALSE(my_convar_bool.getValue<bool>());
    EXPECT_TRUE(check_bool);

    ConVar my_convar_int{"my_convar_int", 2, "", [&check_int](int newValue) {
        check_int = true;
    }};
    my_convar_int.setValue(3);
    EXPECT_EQ(my_convar_int.getValue<int>(), 3);
    EXPECT_TRUE(check_int);

    ConVar my_convar_float{"my_convar_float", 4.5f, "", [&check_float](float newValue) {
        check_float = true;
    }};
    my_convar_float.setValue(2.5f);
    EXPECT_FLOAT_EQ(my_convar_float.getValue<float>(), 2.5f);
    EXPECT_TRUE(check_float);
}

TEST(ConVar, typesConstructedCorrectly) {
    ConVar my_convar_bool{"my_convar_bool", true};
    EXPECT_EQ(my_convar_bool.getType(), ConVarType::BOOLEAN);

    ConVar my_convar_int{"my_convar_int", 0};
    EXPECT_EQ(my_convar_int.getType(), ConVarType::INTEGER);

    ConVar my_convar_float{"my_convar_float", 0.f};
    EXPECT_EQ(my_convar_float.getType(), ConVarType::FLOAT);
}

TEST(ConVar, typeConversionsGetter) {
    ConVar my_convar_bool{"my_convar_bool", false};
    EXPECT_FALSE(my_convar_bool.getValue<bool>());
    EXPECT_EQ(my_convar_bool.getValue<int>(), 0);
    EXPECT_FLOAT_EQ(my_convar_bool.getValue<float>(), 0.f);

    ConVar my_convar_int{"my_convar_int", 2};
    EXPECT_TRUE(my_convar_int.getValue<bool>());
    EXPECT_EQ(my_convar_int.getValue<int>(), 2);
    EXPECT_FLOAT_EQ(my_convar_int.getValue<float>(), 2.f);

    ConVar my_convar_float{"my_convar_float", 1.5f};
    EXPECT_TRUE(my_convar_float.getValue<bool>());
    EXPECT_EQ(my_convar_float.getValue<int>(), 1);
    EXPECT_FLOAT_EQ(my_convar_float.getValue<float>(), 1.5f);
}

TEST(ConVar, typeConversionsSetter) {
    ConVar my_convar_bool{"my_convar_bool", false};
    my_convar_bool.setValue(15);
    EXPECT_TRUE(my_convar_bool.getValue<bool>());
    my_convar_bool.setValue(0.f);
    EXPECT_FALSE(my_convar_bool.getValue<bool>());

    ConVar my_convar_int{"my_convar_int", 2};
    my_convar_int.setValue(false);
    EXPECT_EQ(my_convar_int.getValue<int>(), 0);
    my_convar_int.setValue(10.f);
    EXPECT_EQ(my_convar_int.getValue<int>(), 10);

    ConVar my_convar_float{"my_convar_float", 1.5f};
    my_convar_float.setValue(false);
    EXPECT_FLOAT_EQ(my_convar_float.getValue<float>(), 0.f);
    my_convar_float.setValue(4);
    EXPECT_FLOAT_EQ(my_convar_float.getValue<float>(), 4.f);
}

TEST(ConVarReference, useExistingConVar) {
    ConVar my_convar{"my_convar", 0};
    ConVarReference my_convar_ref{"my_convar"};
    ASSERT_EQ(my_convar_ref.get(), &my_convar);
    ASSERT_EQ(my_convar_ref->getValue<int>(), my_convar.getValue<int>());

    my_convar.setValue(1);
    ASSERT_EQ(my_convar_ref->getValue<int>(), 1);
    my_convar_ref->setValue(2);
    ASSERT_EQ(my_convar.getValue<int>(), 2);
}

TEST(ConVarRegistry, hasConVar) {
    EXPECT_FALSE(ConVarRegistry::hasConVar("my_convar"));

    {
        ConVar my_convar{"my_convar", 0.f};
        EXPECT_TRUE(ConVarRegistry::hasConVar("my_convar"));
    }

    EXPECT_FALSE(ConVarRegistry::hasConVar("my_convar"));
}
