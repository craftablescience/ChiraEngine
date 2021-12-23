#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <resource/provider/filesystemResourceProvider.h>
#include <resource/stringResource.h>

using namespace chira;

TEST(filesystemResourceProvider, getStringResource) {
    resource::addResourceProvider(new filesystemResourceProvider{"test"});
    auto missing = resource::getResource<stringResource>("file://string_resource_test.txt");
    EXPECT_EQ(missing.useCount(), 2);
    EXPECT_STREQ(missing->getString().c_str(), "test");
    resource::removeResource(missing->getIdentifier());
    resource::discardAll();
}

TEST(filesystemResourceProvider, getResourcePath) {
    auto path1 = filesystemResourceProvider::getResourcePath(R"(C:\this\is\a\path\)" + FILESYSTEM_ROOT_FOLDER + R"(\test\files\file.txt)");
    EXPECT_STREQ(path1.c_str(), (FILESYSTEM_PROVIDER_NAME + RESOURCE_ID_SEPARATOR.data() + "files/file.txt").c_str());

    auto path2 = filesystemResourceProvider::getResourcePath(R"(/this/is/a/path/)" + FILESYSTEM_ROOT_FOLDER + R"(/test/files/file.txt)");
    EXPECT_STREQ(path2.c_str(), (FILESYSTEM_PROVIDER_NAME + RESOURCE_ID_SEPARATOR.data() + "files/file.txt").c_str());

    auto path3 = filesystemResourceProvider::getResourcePath(R"(C:\this\is\not\a\valid\path\file.txt)");
    EXPECT_STREQ(path3.c_str(), "");

    auto path4 = filesystemResourceProvider::getResourcePath(R"(/this/is/not/a/valid/path/file.txt)");
    EXPECT_STREQ(path4.c_str(), "");
}

#pragma clang diagnostic pop
