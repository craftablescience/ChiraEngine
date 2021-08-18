#include "abstractResource.h"

#include "resourceManager.h"

void abstractResource::removeIfUnused() const {
    resourceManager::removeResourceIfUnused(this->provider, this->name);
}

abstractResource::~abstractResource() {
    this->removeIfUnused();
}
