#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <core/utility/TypeString.h>

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

TEST(TypeString, simple) {
    EXPECT_STREQ(TypeString<decltype(voidParamsNone)>("name")().c_str(), "void name()");
    EXPECT_STREQ(TypeString<decltype(boolParamsNone)>("name")().c_str(), "bool name()");
    EXPECT_STREQ(TypeString<decltype(stringParamsNone)>("name")().c_str(), "string name()");

    EXPECT_STREQ(TypeString<decltype(voidParamsBool)>("name")().c_str(), "void name(bool)");
    EXPECT_STREQ(TypeString<decltype(voidParamsBoolBool)>("name")().c_str(), "void name(bool,bool)");
    EXPECT_STREQ(TypeString<decltype(voidParamsString)>("name")().c_str(), "void name(string)");
    EXPECT_STREQ(TypeString<decltype(voidParamsStringString)>("name")().c_str(), "void name(string,string)");
    EXPECT_STREQ(TypeString<decltype(voidParamsBoolString)>("name")().c_str(), "void name(bool,string)");

    EXPECT_STREQ(TypeString<decltype(boolParamsBool)>("name")().c_str(), "bool name(bool)");
    EXPECT_STREQ(TypeString<decltype(boolParamsBoolBool)>("name")().c_str(), "bool name(bool,bool)");
    EXPECT_STREQ(TypeString<decltype(boolParamsString)>("name")().c_str(), "bool name(string)");
    EXPECT_STREQ(TypeString<decltype(boolParamsStringString)>("name")().c_str(), "bool name(string,string)");
    EXPECT_STREQ(TypeString<decltype(boolParamsBoolString)>("name")().c_str(), "bool name(bool,string)");

    EXPECT_STREQ(TypeString<decltype(stringParamsBool)>("name")().c_str(), "string name(bool)");
    EXPECT_STREQ(TypeString<decltype(stringParamsBoolBool)>("name")().c_str(), "string name(bool,bool)");
    EXPECT_STREQ(TypeString<decltype(stringParamsString)>("name")().c_str(), "string name(string)");
    EXPECT_STREQ(TypeString<decltype(stringParamsStringString)>("name")().c_str(), "string name(string,string)");
    EXPECT_STREQ(TypeString<decltype(stringParamsBoolString)>("name")().c_str(), "string name(bool,string)");
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

TEST(TypeString, constRef) {
    EXPECT_STREQ(TypeString<decltype(boolConstParamsNone)>("name")().c_str(), "const bool name()");
    EXPECT_STREQ(TypeString<decltype(stringConstParamsNone)>("name")().c_str(), "const string name()");

    EXPECT_STREQ(TypeString<decltype(voidParamsConstBool)>("name")().c_str(), "void name(const bool &in)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstBoolBool)>("name")().c_str(), "void name(const bool &in,const bool &in)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstString)>("name")().c_str(), "void name(const string &in)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstStringString)>("name")().c_str(), "void name(const string &in,const string &in)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstBoolString)>("name")().c_str(), "void name(const bool &in,const string &in)");

    EXPECT_STREQ(TypeString<decltype(boolConstParamsConstBool)>("name")().c_str(), "const bool & name(const bool &in)");
    EXPECT_STREQ(TypeString<decltype(boolConstParamsConstBoolBool)>("name")().c_str(), "const bool & name(const bool &in,const bool &in)");
    EXPECT_STREQ(TypeString<decltype(boolConstParamsConstString)>("name")().c_str(), "const bool & name(const string &in)");
    EXPECT_STREQ(TypeString<decltype(boolConstParamsConstStringString)>("name")().c_str(), "const bool & name(const string &in,const string &in)");
    EXPECT_STREQ(TypeString<decltype(boolConstParamsConstBoolString)>("name")().c_str(), "const bool & name(const bool &in,const string &in)");

    EXPECT_STREQ(TypeString<decltype(stringConstParamsConstBool)>("name")().c_str(), "const string & name(const bool &in)");
    EXPECT_STREQ(TypeString<decltype(stringConstParamsConstBoolBool)>("name")().c_str(), "const string & name(const bool &in,const bool &in)");
    EXPECT_STREQ(TypeString<decltype(stringConstParamsConstString)>("name")().c_str(), "const string & name(const string &in)");
    EXPECT_STREQ(TypeString<decltype(stringConstParamsConstStringString)>("name")().c_str(), "const string & name(const string &in,const string &in)");
    EXPECT_STREQ(TypeString<decltype(stringConstParamsConstBoolString)>("name")().c_str(), "const string & name(const bool &in,const string &in)");
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

TEST(TypeString, ptr) {
    EXPECT_STREQ(TypeString<decltype(boolPtrParamsNone)>("name")().c_str(), "bool & name()");
    EXPECT_STREQ(TypeString<decltype(stringPtrParamsNone)>("name")().c_str(), "string @ name()");

    EXPECT_STREQ(TypeString<decltype(voidParamsPtrBool)>("name")().c_str(), "void name(bool &out)");
    EXPECT_STREQ(TypeString<decltype(voidParamsPtrBoolBool)>("name")().c_str(), "void name(bool &out,bool &out)");
    EXPECT_STREQ(TypeString<decltype(voidParamsPtrString)>("name")().c_str(), "void name(string @)");
    EXPECT_STREQ(TypeString<decltype(voidParamsPtrStringString)>("name")().c_str(), "void name(string @,string @)");
    EXPECT_STREQ(TypeString<decltype(voidParamsPtrBoolString)>("name")().c_str(), "void name(bool &out,string @)");

    EXPECT_STREQ(TypeString<decltype(boolPtrParamsPtrBool)>("name")().c_str(), "bool & name(bool &out)");
    EXPECT_STREQ(TypeString<decltype(boolPtrParamsPtrBoolBool)>("name")().c_str(), "bool & name(bool &out,bool &out)");
    EXPECT_STREQ(TypeString<decltype(boolPtrParamsPtrString)>("name")().c_str(), "bool & name(string @)");
    EXPECT_STREQ(TypeString<decltype(boolPtrParamsPtrStringString)>("name")().c_str(), "bool & name(string @,string @)");
    EXPECT_STREQ(TypeString<decltype(boolPtrParamsPtrBoolString)>("name")().c_str(), "bool & name(bool &out,string @)");

    EXPECT_STREQ(TypeString<decltype(stringPtrParamsPtrBool)>("name")().c_str(), "string @ name(bool &out)");
    EXPECT_STREQ(TypeString<decltype(stringPtrParamsPtrBoolBool)>("name")().c_str(), "string @ name(bool &out,bool &out)");
    EXPECT_STREQ(TypeString<decltype(stringPtrParamsPtrString)>("name")().c_str(), "string @ name(string @)");
    EXPECT_STREQ(TypeString<decltype(stringPtrParamsPtrStringString)>("name")().c_str(), "string @ name(string @,string @)");
    EXPECT_STREQ(TypeString<decltype(stringPtrParamsPtrBoolString)>("name")().c_str(), "string @ name(bool &out,string @)");
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

TEST(TypeString, constPtr) {
    EXPECT_STREQ(TypeString<decltype(boolConstPtrParamsNone)>("name")().c_str(), "const bool & name()");
    EXPECT_STREQ(TypeString<decltype(stringConstPtrParamsNone)>("name")().c_str(), "const string @ name()");

    EXPECT_STREQ(TypeString<decltype(voidParamsConstPtrBool)>("name")().c_str(), "void name(const bool &out)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstPtrBoolBool)>("name")().c_str(), "void name(const bool &out,const bool &out)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstPtrString)>("name")().c_str(), "void name(const string @)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstPtrStringString)>("name")().c_str(), "void name(const string @,const string @)");
    EXPECT_STREQ(TypeString<decltype(voidParamsConstPtrBoolString)>("name")().c_str(), "void name(const bool &out,const string @)");

    EXPECT_STREQ(TypeString<decltype(boolConstPtrParamsConstPtrBool)>("name")().c_str(), "const bool & name(const bool &out)");
    EXPECT_STREQ(TypeString<decltype(boolConstPtrParamsConstPtrBoolBool)>("name")().c_str(), "const bool & name(const bool &out,const bool &out)");
    EXPECT_STREQ(TypeString<decltype(boolConstPtrParamsConstPtrString)>("name")().c_str(), "const bool & name(const string @)");
    EXPECT_STREQ(TypeString<decltype(boolConstPtrParamsConstPtrStringString)>("name")().c_str(), "const bool & name(const string @,const string @)");
    EXPECT_STREQ(TypeString<decltype(boolConstPtrParamsConstPtrBoolString)>("name")().c_str(), "const bool & name(const bool &out,const string @)");

    EXPECT_STREQ(TypeString<decltype(stringConstPtrParamsConstPtrBool)>("name")().c_str(), "const string @ name(const bool &out)");
    EXPECT_STREQ(TypeString<decltype(stringConstPtrParamsConstPtrBoolBool)>("name")().c_str(), "const string @ name(const bool &out,const bool &out)");
    EXPECT_STREQ(TypeString<decltype(stringConstPtrParamsConstPtrString)>("name")().c_str(), "const string @ name(const string @)");
    EXPECT_STREQ(TypeString<decltype(stringConstPtrParamsConstPtrStringString)>("name")().c_str(), "const string @ name(const string @,const string @)");
    EXPECT_STREQ(TypeString<decltype(stringConstPtrParamsConstPtrBoolString)>("name")().c_str(), "const string @ name(const bool &out,const string @)");
}
