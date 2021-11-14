#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <utility/string/stringRemove.h>
#include <utility/string/stringStrip.h>

using namespace chira;

TEST(stringRemove, remove) {
    std::string test1 = "Test String", test2 = "Test String", test3 = "Test String";
    remove(test1, 'S');
    remove(test2, 't');
    remove(test3, 's');
    EXPECT_STREQ(test1.c_str(), "Test tring");
    EXPECT_STREQ(test2.c_str(), "Tes Sring");
    EXPECT_STREQ(test3.c_str(), "Tet String");
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

#pragma clang diagnostic pop
