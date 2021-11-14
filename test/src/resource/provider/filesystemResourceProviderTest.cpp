#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <resource/provider/filesystemResourceProvider.h>
#include <resource/resourceManager.h>
#include <resource/stringResource.h>

using namespace chira;

TEST(filesystemResourceProvider, getStringResource) {
    resourceManager::addResourceProvider("file", new filesystemResourceProvider{"file", "resources/engine"});
    auto missing = resourceManager::getResource<stringResource>("file://test/string_resource_test.txt");
    EXPECT_EQ(missing.use_count(), 2);
    EXPECT_EQ(missing->getString().c_str(), "test");
    resourceManager::removeResource(missing->getIdentifier());
    resourceManager::discardAll();
}

#pragma clang diagnostic pop
