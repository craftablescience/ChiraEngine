#include <gtest/gtest.h>

#include <utility/String.h>

using namespace chira;

TEST(StringContains, startsWith) {
    std::string test = "StartEnd";
    EXPECT_TRUE(String::startsWith(test, "Start"));
    EXPECT_TRUE(String::startsWith(test, 'S'));
    EXPECT_FALSE(String::startsWith(test, "xxx"));
    EXPECT_FALSE(String::startsWith(test, 'x'));
}

TEST(StringContains, endsWith) {
    std::string test = "StartEnd";
    EXPECT_TRUE(String::endsWith(test, "End"));
    EXPECT_TRUE(String::endsWith(test, 'd'));
    EXPECT_FALSE(String::endsWith(test, "xxx"));
    EXPECT_FALSE(String::endsWith(test, 'x'));
}

TEST(StringContains, contains) {
    std::string test = "StartEnd";
    EXPECT_TRUE(String::contains(test, "tEn"));
    EXPECT_TRUE(String::contains(test, 'E'));
    EXPECT_FALSE(String::contains(test, "txn"));
    EXPECT_FALSE(String::contains(test, 'x'));
}

TEST(StringRemove, remove) {
    std::string test1 = "Test String", test2 = "Test String", test3 = "Test String";
    String::remove(test1, 'S');
    String::remove(test2, 't');
    String::remove(test3, 's');
    EXPECT_STREQ(test1.c_str(), "Test tring");
    EXPECT_STREQ(test2.c_str(), "Tes Sring");
    EXPECT_STREQ(test3.c_str(), "Tet String");
}

TEST(StringSplit, splitString) {
    auto vec1 = String::split("TestString1:TestString2:TestString3", ':');
    ASSERT_EQ(vec1.size(), 3);
    EXPECT_STREQ(vec1[0].c_str(), "TestString1");
    EXPECT_STREQ(vec1[1].c_str(), "TestString2");
    EXPECT_STREQ(vec1[2].c_str(), "TestString3");

    auto vec2 = String::split("Test String 1,Test String 2,Test String 3", ',');
    ASSERT_EQ(vec2.size(), 3);
    EXPECT_STREQ(vec2[0].c_str(), "Test String 1");
    EXPECT_STREQ(vec2[1].c_str(), "Test String 2");
    EXPECT_STREQ(vec2[2].c_str(), "Test String 3");
}

TEST(StringStrip, stripWhitespace) {
    EXPECT_STREQ(String::stripLeft("\t\n\r\v\f test").c_str(), "test");
    EXPECT_STREQ(String::stripRight("test \t\n\r\v\f").c_str(), "test");
    EXPECT_STREQ(String::strip("\t\n\r\v\f test \t\n\r\v\f").c_str(), "test");
}

TEST(StringStrip, stripString) {
    EXPECT_STREQ(String::stripLeft(" test", " ").c_str(), "test");
    EXPECT_STREQ(String::stripRight("test ", " ").c_str(), "test");
    EXPECT_STREQ(String::strip(" test ", " ").c_str(), "test");
}
