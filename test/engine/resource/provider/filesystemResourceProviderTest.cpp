#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <resource/provider/filesystemResourceProvider.h>
#include <resource/stringResource.h>

using namespace chira;

TEST(filesystemResourceProvider, getStringResource) {
    Resource::addResourceProvider(new FilesystemResourceProvider{"test"});
    auto missing = Resource::getResource<StringResource>("file://string_resource_test.txt");
    EXPECT_EQ(missing.useCount(), 2);
    EXPECT_STREQ(missing->getString().c_str(), "test");
    Resource::removeResource(missing->getIdentifier());
    Resource::discardAll();
}

TEST(filesystemResourceProvider, getResourceIdentifier) {
    auto path1 = FilesystemResourceProvider::getResourceIdentifier(R"(C:\this\is\a\path\)" + FILESYSTEM_ROOT_FOLDER + R"(\test\files\file.txt)");
    EXPECT_STREQ(path1.c_str(), (FILESYSTEM_PROVIDER_NAME + RESOURCE_ID_SEPARATOR.data() + "files/file.txt").c_str());

    auto path2 = FilesystemResourceProvider::getResourceIdentifier("/this/is/a/path/" + FILESYSTEM_ROOT_FOLDER + "/test/files/file.txt");
    EXPECT_STREQ(path2.c_str(), (FILESYSTEM_PROVIDER_NAME + RESOURCE_ID_SEPARATOR.data() + "files/file.txt").c_str());

    auto path3 = FilesystemResourceProvider::getResourceIdentifier(R"(C:\this\is\not\a\valid\path\file.txt)");
    EXPECT_STREQ(path3.c_str(), "");

    auto path4 = FilesystemResourceProvider::getResourceIdentifier("/this/is/not/a/valid/path/file.txt");
    EXPECT_STREQ(path4.c_str(), "");
}

#pragma clang diagnostic pop
