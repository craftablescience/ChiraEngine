#include <gtest/gtest.h>

#include <resource/Properties.h>

using namespace chira;

struct SimpleStructWithProps {
    int x = 0, y = 12, z = -72;
    std::string name{"hello"};
    bool b = true;

    CHIRA_PROPS (
            CHIRA_PROP(SimpleStructWithProps, x),
            CHIRA_PROP(SimpleStructWithProps, y),
            CHIRA_PROP_NAMED(SimpleStructWithProps, z, third_number),
            CHIRA_PROP(SimpleStructWithProps, name),
            CHIRA_PROP(SimpleStructWithProps, b)
    );
};

TEST(Properties, simpleDeserialize) {
    SimpleStructWithProps test{};
    Serialize::fromJSON(&test, nlohmann::json::parse(R"({
      "x": -45,
      "y": 0,
      "z": "no",
      "third_number": 23,
      "name": "world",
      "b": false
    })"));
    EXPECT_EQ(test.x, -45);
    EXPECT_EQ(test.y, 0);
    EXPECT_EQ(test.z, 23);
    EXPECT_STREQ(test.name.c_str(), "world");
    EXPECT_FALSE(test.b);
}

TEST(Properties, simpleSerialize) {
    SimpleStructWithProps test{};
    nlohmann::json testJson = Serialize::toJSON(test);
    EXPECT_EQ(testJson["x"], 0);
    EXPECT_EQ(testJson["y"], 12);
    EXPECT_EQ(testJson["third_number"], -72);
    EXPECT_STREQ(testJson["name"].get<std::string>().c_str(), "hello");
    EXPECT_TRUE(testJson["b"]);
}

//todo: check vector, unordered_map work, implement them if they don't
//      especially vector
