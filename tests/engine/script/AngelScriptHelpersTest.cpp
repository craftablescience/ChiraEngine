#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <script/AngelScriptHelpers.h>

using namespace chira;

static void voidParamsNone();
static bool boolParamsNone();
static std::string stringParamsNone();

static void voidParamsBool(bool);
static void voidParamsBoolBool(bool, bool);
static void voidParamsString(std::string);
static void voidParamsStringString(std::string, std::string);
static void voidParamsBoolString(bool, std::string);

static bool boolParamsBool(bool);
static bool boolParamsBoolBool(bool, bool);
static bool boolParamsString(std::string);
static bool boolParamsStringString(std::string, std::string);
static bool boolParamsBoolString(bool, std::string);

static std::string stringParamsBool(bool);
static std::string stringParamsBoolBool(bool, bool);
static std::string stringParamsString(std::string);
static std::string stringParamsStringString(std::string, std::string);
static std::string stringParamsBoolString(bool, std::string);

TEST(AngelScriptHelpers, asTypeStringSimple) {
    EXPECT_STREQ(asTypeString<decltype(voidParamsNone)>("name")().c_str(), "void name()");
    EXPECT_STREQ(asTypeString<decltype(boolParamsNone)>("name")().c_str(), "bool name()");
    EXPECT_STREQ(asTypeString<decltype(stringParamsNone)>("name")().c_str(), "string name()");

    EXPECT_STREQ(asTypeString<decltype(voidParamsBool)>("name")().c_str(), "void name(bool)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsBoolBool)>("name")().c_str(), "void name(bool,bool)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsString)>("name")().c_str(), "void name(string)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsStringString)>("name")().c_str(), "void name(string,string)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsBoolString)>("name")().c_str(), "void name(bool,string)");

    EXPECT_STREQ(asTypeString<decltype(boolParamsBool)>("name")().c_str(), "bool name(bool)");
    EXPECT_STREQ(asTypeString<decltype(boolParamsBoolBool)>("name")().c_str(), "bool name(bool,bool)");
    EXPECT_STREQ(asTypeString<decltype(boolParamsString)>("name")().c_str(), "bool name(string)");
    EXPECT_STREQ(asTypeString<decltype(boolParamsStringString)>("name")().c_str(), "bool name(string,string)");
    EXPECT_STREQ(asTypeString<decltype(boolParamsBoolString)>("name")().c_str(), "bool name(bool,string)");

    EXPECT_STREQ(asTypeString<decltype(stringParamsBool)>("name")().c_str(), "string name(bool)");
    EXPECT_STREQ(asTypeString<decltype(stringParamsBoolBool)>("name")().c_str(), "string name(bool,bool)");
    EXPECT_STREQ(asTypeString<decltype(stringParamsString)>("name")().c_str(), "string name(string)");
    EXPECT_STREQ(asTypeString<decltype(stringParamsStringString)>("name")().c_str(), "string name(string,string)");
    EXPECT_STREQ(asTypeString<decltype(stringParamsBoolString)>("name")().c_str(), "string name(bool,string)");
}

static const bool boolConstParamsNone();
static const std::string stringConstParamsNone();

static void voidParamsConstBool(const bool&);
static void voidParamsConstBoolBool(const bool&, const bool&);
static void voidParamsConstString(const std::string&);
static void voidParamsConstStringString(const std::string&, const std::string&);
static void voidParamsConstBoolString(const bool&, const std::string&);

static const bool& boolConstParamsConstBool(const bool&);
static const bool& boolConstParamsConstBoolBool(const bool&, const bool&);
static const bool& boolConstParamsConstString(const std::string&);
static const bool& boolConstParamsConstStringString(const std::string&, const std::string&);
static const bool& boolConstParamsConstBoolString(const bool&, const std::string&);

static const std::string& stringConstParamsConstBool(const bool&);
static const std::string& stringConstParamsConstBoolBool(const bool&, const bool&);
static const std::string& stringConstParamsConstString(const std::string&);
static const std::string& stringConstParamsConstStringString(const std::string&, const std::string&);
static const std::string& stringConstParamsConstBoolString(const bool&, const std::string&);

TEST(AngelScriptHelpers, asTypeStringConstRef) {
    EXPECT_STREQ(asTypeString<decltype(boolConstParamsNone)>("name")().c_str(), "const bool name()");
    EXPECT_STREQ(asTypeString<decltype(stringConstParamsNone)>("name")().c_str(), "const string name()");

    EXPECT_STREQ(asTypeString<decltype(voidParamsConstBool)>("name")().c_str(), "void name(const bool &in)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstBoolBool)>("name")().c_str(), "void name(const bool &in,const bool &in)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstString)>("name")().c_str(), "void name(const string &in)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstStringString)>("name")().c_str(), "void name(const string &in,const string &in)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstBoolString)>("name")().c_str(), "void name(const bool &in,const string &in)");

    EXPECT_STREQ(asTypeString<decltype(boolConstParamsConstBool)>("name")().c_str(), "const bool & name(const bool &in)");
    EXPECT_STREQ(asTypeString<decltype(boolConstParamsConstBoolBool)>("name")().c_str(), "const bool & name(const bool &in,const bool &in)");
    EXPECT_STREQ(asTypeString<decltype(boolConstParamsConstString)>("name")().c_str(), "const bool & name(const string &in)");
    EXPECT_STREQ(asTypeString<decltype(boolConstParamsConstStringString)>("name")().c_str(), "const bool & name(const string &in,const string &in)");
    EXPECT_STREQ(asTypeString<decltype(boolConstParamsConstBoolString)>("name")().c_str(), "const bool & name(const bool &in,const string &in)");

    EXPECT_STREQ(asTypeString<decltype(stringConstParamsConstBool)>("name")().c_str(), "const string & name(const bool &in)");
    EXPECT_STREQ(asTypeString<decltype(stringConstParamsConstBoolBool)>("name")().c_str(), "const string & name(const bool &in,const bool &in)");
    EXPECT_STREQ(asTypeString<decltype(stringConstParamsConstString)>("name")().c_str(), "const string & name(const string &in)");
    EXPECT_STREQ(asTypeString<decltype(stringConstParamsConstStringString)>("name")().c_str(), "const string & name(const string &in,const string &in)");
    EXPECT_STREQ(asTypeString<decltype(stringConstParamsConstBoolString)>("name")().c_str(), "const string & name(const bool &in,const string &in)");
}

static bool* boolPtrParamsNone();
static std::string* stringPtrParamsNone();

static void voidParamsPtrBool(bool*);
static void voidParamsPtrBoolBool(bool*, bool*);
static void voidParamsPtrString(std::string*);
static void voidParamsPtrStringString(std::string*, std::string*);
static void voidParamsPtrBoolString(bool*, std::string*);

static bool* boolPtrParamsPtrBool(bool*);
static bool* boolPtrParamsPtrBoolBool(bool*, bool*);
static bool* boolPtrParamsPtrString(std::string*);
static bool* boolPtrParamsPtrStringString(std::string*, std::string*);
static bool* boolPtrParamsPtrBoolString(bool*, std::string*);

static std::string* stringPtrParamsPtrBool(bool*);
static std::string* stringPtrParamsPtrBoolBool(bool*, bool*);
static std::string* stringPtrParamsPtrString(std::string*);
static std::string* stringPtrParamsPtrStringString(std::string*, std::string*);
static std::string* stringPtrParamsPtrBoolString(bool*, std::string*);

TEST(AngelScriptHelpers, asTypeStringPtr) {
    EXPECT_STREQ(asTypeString<decltype(boolPtrParamsNone)>("name")().c_str(), "bool & name()");
    EXPECT_STREQ(asTypeString<decltype(stringPtrParamsNone)>("name")().c_str(), "string @ name()");

    EXPECT_STREQ(asTypeString<decltype(voidParamsPtrBool)>("name")().c_str(), "void name(bool &out)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsPtrBoolBool)>("name")().c_str(), "void name(bool &out,bool &out)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsPtrString)>("name")().c_str(), "void name(string @)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsPtrStringString)>("name")().c_str(), "void name(string @,string @)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsPtrBoolString)>("name")().c_str(), "void name(bool &out,string @)");

    EXPECT_STREQ(asTypeString<decltype(boolPtrParamsPtrBool)>("name")().c_str(), "bool & name(bool &out)");
    EXPECT_STREQ(asTypeString<decltype(boolPtrParamsPtrBoolBool)>("name")().c_str(), "bool & name(bool &out,bool &out)");
    EXPECT_STREQ(asTypeString<decltype(boolPtrParamsPtrString)>("name")().c_str(), "bool & name(string @)");
    EXPECT_STREQ(asTypeString<decltype(boolPtrParamsPtrStringString)>("name")().c_str(), "bool & name(string @,string @)");
    EXPECT_STREQ(asTypeString<decltype(boolPtrParamsPtrBoolString)>("name")().c_str(), "bool & name(bool &out,string @)");

    EXPECT_STREQ(asTypeString<decltype(stringPtrParamsPtrBool)>("name")().c_str(), "string @ name(bool &out)");
    EXPECT_STREQ(asTypeString<decltype(stringPtrParamsPtrBoolBool)>("name")().c_str(), "string @ name(bool &out,bool &out)");
    EXPECT_STREQ(asTypeString<decltype(stringPtrParamsPtrString)>("name")().c_str(), "string @ name(string @)");
    EXPECT_STREQ(asTypeString<decltype(stringPtrParamsPtrStringString)>("name")().c_str(), "string @ name(string @,string @)");
    EXPECT_STREQ(asTypeString<decltype(stringPtrParamsPtrBoolString)>("name")().c_str(), "string @ name(bool &out,string @)");
}

static const bool* boolConstPtrParamsNone();
static const std::string* stringConstPtrParamsNone();

static void voidParamsConstPtrBool(const bool*);
static void voidParamsConstPtrBoolBool(const bool*, const bool*);
static void voidParamsConstPtrString(const std::string*);
static void voidParamsConstPtrStringString(const std::string*, const std::string*);
static void voidParamsConstPtrBoolString(const bool*, const std::string*);

static const bool* boolConstPtrParamsConstPtrBool(const bool*);
static const bool* boolConstPtrParamsConstPtrBoolBool(const bool*, const bool*);
static const bool* boolConstPtrParamsConstPtrString(const std::string*);
static const bool* boolConstPtrParamsConstPtrStringString(const std::string*, const std::string*);
static const bool* boolConstPtrParamsConstPtrBoolString(const bool*, const std::string*);

static const std::string* stringConstPtrParamsConstPtrBool(const bool*);
static const std::string* stringConstPtrParamsConstPtrBoolBool(const bool*, const bool*);
static const std::string* stringConstPtrParamsConstPtrString(const std::string*);
static const std::string* stringConstPtrParamsConstPtrStringString(const std::string*, const std::string*);
static const std::string* stringConstPtrParamsConstPtrBoolString(const bool*, const std::string*);

TEST(AngelScriptHelpers, asTypeStringConstPtr) {
    EXPECT_STREQ(asTypeString<decltype(boolConstPtrParamsNone)>("name")().c_str(), "const bool & name()");
    EXPECT_STREQ(asTypeString<decltype(stringConstPtrParamsNone)>("name")().c_str(), "const string @ name()");

    EXPECT_STREQ(asTypeString<decltype(voidParamsConstPtrBool)>("name")().c_str(), "void name(const bool &out)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstPtrBoolBool)>("name")().c_str(), "void name(const bool &out,const bool &out)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstPtrString)>("name")().c_str(), "void name(const string @)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstPtrStringString)>("name")().c_str(), "void name(const string @,const string @)");
    EXPECT_STREQ(asTypeString<decltype(voidParamsConstPtrBoolString)>("name")().c_str(), "void name(const bool &out,const string @)");

    EXPECT_STREQ(asTypeString<decltype(boolConstPtrParamsConstPtrBool)>("name")().c_str(), "const bool & name(const bool &out)");
    EXPECT_STREQ(asTypeString<decltype(boolConstPtrParamsConstPtrBoolBool)>("name")().c_str(), "const bool & name(const bool &out,const bool &out)");
    EXPECT_STREQ(asTypeString<decltype(boolConstPtrParamsConstPtrString)>("name")().c_str(), "const bool & name(const string @)");
    EXPECT_STREQ(asTypeString<decltype(boolConstPtrParamsConstPtrStringString)>("name")().c_str(), "const bool & name(const string @,const string @)");
    EXPECT_STREQ(asTypeString<decltype(boolConstPtrParamsConstPtrBoolString)>("name")().c_str(), "const bool & name(const bool &out,const string @)");

    EXPECT_STREQ(asTypeString<decltype(stringConstPtrParamsConstPtrBool)>("name")().c_str(), "const string @ name(const bool &out)");
    EXPECT_STREQ(asTypeString<decltype(stringConstPtrParamsConstPtrBoolBool)>("name")().c_str(), "const string @ name(const bool &out,const bool &out)");
    EXPECT_STREQ(asTypeString<decltype(stringConstPtrParamsConstPtrString)>("name")().c_str(), "const string @ name(const string @)");
    EXPECT_STREQ(asTypeString<decltype(stringConstPtrParamsConstPtrStringString)>("name")().c_str(), "const string @ name(const string @,const string @)");
    EXPECT_STREQ(asTypeString<decltype(stringConstPtrParamsConstPtrBoolString)>("name")().c_str(), "const string @ name(const bool &out,const string @)");
}
