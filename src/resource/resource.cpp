#include "resource.h"

#include "resourceManager.h"

using namespace chira;

resource::~resource() {
    resourceManager::removeResource(this->identifier);
}
