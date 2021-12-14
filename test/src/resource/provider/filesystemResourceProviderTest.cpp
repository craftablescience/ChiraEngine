#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <resource/provider/filesystemResourceProvider.h>
#include <resource/stringResource.h>

using namespace chira;

TEST(filesystemResourceProvider, getStringResource) {
    resource::addResourceProvider("file", new filesystemResourceProvider{"file", "resources/engine"});
    auto missing = resource::getResource<stringResource>("file://test/string_resource_test.txt");
    EXPECT_EQ(missing.useCount(), 2);
    EXPECT_STREQ(missing->getString().c_str(), "test");
    resource::removeResource(missing->getIdentifier());
    resource::discardAll();
}

#pragma clang diagnostic pop
