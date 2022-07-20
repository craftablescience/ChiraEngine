#include <gtest/gtest.h>

#include <utility/ConVar.h>

using namespace chira;

TEST(ConVar, createConVar) {
    ConVar my_convar{"my_convar", 0.f};
    ASSERT_FLOAT_EQ(my_convar.getValue(), 0.f);

    my_convar.setValue(1.f);
    ASSERT_FLOAT_EQ(my_convar.getValue(), 1.f);
}

TEST(ConVar, useConVarSetValueCallback) {
    bool check = false;
    ConVar my_convar{"my_convar", 0.f, "", [&check](float newValue) {
        check = true;
    }};
    my_convar.setValue(1.f);
    ASSERT_FLOAT_EQ(my_convar.getValue(), 1.f);
    ASSERT_TRUE(check);
}

TEST(ConVarReference, useExistingConVar) {
    ConVar my_convar{"my_convar", 0.f};
    ConVarReference my_convar_ref{"my_convar"};
    ASSERT_EQ(my_convar_ref.get(), &my_convar);
    ASSERT_FLOAT_EQ(my_convar_ref->getValue(), my_convar.getValue());

    my_convar.setValue(1.f);
    ASSERT_FLOAT_EQ(my_convar_ref->getValue(), 1.f);
    my_convar_ref->setValue(2.f);
    ASSERT_FLOAT_EQ(my_convar.getValue(), 2.f);
}

TEST(ConVarRegistry, hasConVar) {
    EXPECT_FALSE(ConVarRegistry::hasConVar("my_convar"));

    {
        ConVar my_convar{"my_convar", 0.f};
        EXPECT_TRUE(ConVarRegistry::hasConVar("my_convar"));
    }

    EXPECT_FALSE(ConVarRegistry::hasConVar("my_convar"));
}
