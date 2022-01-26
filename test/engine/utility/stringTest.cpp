#include <gtest/gtest.h>

#include <utility/string/stringContains.h>
#include <utility/string/stringRemove.h>
#include <utility/string/stringSplit.h>
#include <utility/string/stringStrip.h>

using namespace chira;

TEST(stringContains, startsWith) {
    std::string test = "StartEnd";
    EXPECT_TRUE(startsWith(test, "Start"));
    EXPECT_TRUE(startsWith(test, 'S'));
    EXPECT_FALSE(startsWith(test, "xxx"));
    EXPECT_FALSE(startsWith(test, 'x'));
}

TEST(stringContains, endsWith) {
    std::string test = "StartEnd";
    EXPECT_TRUE(endsWith(test, "End"));
    EXPECT_TRUE(endsWith(test, 'd'));
    EXPECT_FALSE(endsWith(test, "xxx"));
    EXPECT_FALSE(endsWith(test, 'x'));
}

TEST(stringContains, contains) {
    std::string test = "StartEnd";
    EXPECT_TRUE(contains(test, "tEn"));
    EXPECT_TRUE(contains(test, 'E'));
    EXPECT_FALSE(contains(test, "txn"));
    EXPECT_FALSE(contains(test, 'x'));
}

TEST(stringRemove, remove) {
    std::string test1 = "Test String", test2 = "Test String", test3 = "Test String";
    remove(test1, 'S');
    remove(test2, 't');
    remove(test3, 's');
    EXPECT_STREQ(test1.c_str(), "Test tring");
    EXPECT_STREQ(test2.c_str(), "Tes Sring");
    EXPECT_STREQ(test3.c_str(), "Tet String");
}

TEST(stringSplit, splitString) {
    auto vec1 = split("TestString1:TestString2:TestString3", ':');
    ASSERT_EQ(vec1.size(), 3);
    EXPECT_STREQ(vec1[0].c_str(), "TestString1");
    EXPECT_STREQ(vec1[1].c_str(), "TestString2");
    EXPECT_STREQ(vec1[2].c_str(), "TestString3");

    auto vec2 = split("Test String 1,Test String 2,Test String 3", ',');
    ASSERT_EQ(vec2.size(), 3);
    EXPECT_STREQ(vec2[0].c_str(), "Test String 1");
    EXPECT_STREQ(vec2[1].c_str(), "Test String 2");
    EXPECT_STREQ(vec2[2].c_str(), "Test String 3");
}

TEST(stringStrip, stripWhitespace) {
    EXPECT_STREQ(stripLeft("\t\n\r\v\f test").c_str(), "test");
    EXPECT_STREQ(stripRight("test \t\n\r\v\f").c_str(), "test");
    EXPECT_STREQ(strip("\t\n\r\v\f test \t\n\r\v\f").c_str(), "test");
}

TEST(stringStrip, stripString) {
    EXPECT_STREQ(stripLeft(" test", " ").c_str(), "test");
    EXPECT_STREQ(stripRight("test ", " ").c_str(), "test");
    EXPECT_STREQ(strip(" test ", " ").c_str(), "test");
}

TEST(stringStrip, stringWhitespaceModifyOriginal) {
    std::string test1 = "\t\n\r\v\f test", test2 = "test \t\n\r\v\f", test3 = "\t\n\r\v\f test \t\n\r\v\f";
    stripLeftModify(test1);
    stripRightModify(test2);
    stripModify(test3);
    EXPECT_STREQ(test1.c_str(), "test");
    EXPECT_STREQ(test2.c_str(), "test");
    EXPECT_STREQ(test3.c_str(), "test");
}

TEST(stringStrip, stringStripModifyOriginal) {
    std::string test1 = " test", test2 = "test ", test3 = " test ";
    stripLeftModify(test1, " ");
    stripRightModify(test2, " ");
    stripModify(test3, " ");
    EXPECT_STREQ(test1.c_str(), "test");
    EXPECT_STREQ(test2.c_str(), "test");
    EXPECT_STREQ(test3.c_str(), "test");
}
