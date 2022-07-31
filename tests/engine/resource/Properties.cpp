#include <gtest/gtest.h>

#include <vector>
#include <unordered_map>

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
    auto test = Serialize::fromJSON<SimpleStructWithProps>(nlohmann::json::parse(R"({
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

struct ComplexStructWithProps {
    std::vector<int> list{1, 2, 5, 6};
    std::unordered_map<std::string,bool> dict{ // python 👍
            {"first", true},
            {"second", false}
    };

    CHIRA_PROPS (
            CHIRA_PROP(ComplexStructWithProps, list),
            CHIRA_PROP(ComplexStructWithProps, dict)
    );
};

TEST(Properties, complexDeserialize) {
    auto test = Serialize::fromJSON<ComplexStructWithProps>(nlohmann::json::parse(R"({
        "list": [18, 34, -89],
        "dict": {"first": false, "third": true}
    })"));

    ASSERT_EQ(test.list.size(), 3);
    EXPECT_EQ(test.list[0], 18);
    EXPECT_EQ(test.list[1], 34);
    EXPECT_EQ(test.list[2], -89);

    EXPECT_FALSE(test.dict["first"]);
    EXPECT_FALSE(test.dict.contains("second"));
    ASSERT_TRUE(test.dict.contains("third"));
    EXPECT_TRUE(test.dict["third"]);
}

TEST(Properties, complexSerialize) {
    ComplexStructWithProps test{};
    nlohmann::json testJson = Serialize::toJSON(test);

    ASSERT_TRUE(testJson.contains("list"));
    ASSERT_EQ(testJson["list"].size(), 4);
    EXPECT_EQ(testJson["list"][0], 1);
    EXPECT_EQ(testJson["list"][1], 2);
    EXPECT_EQ(testJson["list"][2], 5);
    EXPECT_EQ(testJson["list"][3], 6);

    ASSERT_TRUE(testJson.contains("dict"));
    ASSERT_EQ(testJson["dict"].size(), 2);
    EXPECT_TRUE(testJson["dict"]["first"]);
    EXPECT_FALSE(testJson["dict"]["second"]);
}

struct StructWithGettersAndSettersForProps {
    int a = 4;
    bool b = true;
    std::string c{"hello"};

    bool aWasGot = false;
    int getA() {
        this->aWasGot = true;
        return this->a + 1;
    }

    bool bWasSet = false;
    void setB(bool b_) {
        this->b = !b_;
        this->bWasSet = true;
    }

    bool cWasGot = false, cWasSet = false;
    std::string getC() {
        this->cWasGot = true;
        return this->c + " getter";
    }
    void setC(std::string c_) {
        this->c = std::move(c_);
        this->c += " setter";
        this->cWasSet = true;
    }

    CHIRA_PROPS (
            CHIRA_PROP_GET(StructWithGettersAndSettersForProps, a, getA),
            CHIRA_PROP_SET(StructWithGettersAndSettersForProps, b, setB),
            CHIRA_PROP_GET_SET(StructWithGettersAndSettersForProps, c, getC, setC)
    );
};

TEST(Properties, gettersAndSetters) {
    StructWithGettersAndSettersForProps test1{};
    nlohmann::json testJson = Serialize::toJSON(test1);
    EXPECT_TRUE(test1.aWasGot);
    EXPECT_EQ(testJson["a"], 5);
    EXPECT_TRUE(testJson["b"]);
    EXPECT_TRUE(test1.cWasGot);
    EXPECT_STREQ(testJson["c"].get<std::string>().c_str(), "hello getter");

    auto test2 = Serialize::fromJSON<StructWithGettersAndSettersForProps>(nlohmann::json::parse(R"({
        "a": 2,
        "b": true,
        "c": "inside"
    })"));
    EXPECT_EQ(test2.a, 2);
    EXPECT_TRUE(test2.bWasSet);
    EXPECT_FALSE(test2.b);
    EXPECT_TRUE(test2.cWasSet);
    EXPECT_STREQ(test2.c.c_str(), "inside setter");
}
