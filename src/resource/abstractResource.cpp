#include "abstractResource.h"

#include "resourceManager.h"

void abstractResource::removeIfUnused() const {
    resourceManager::removeIfUnused(this->provider, this->name);
}

abstractResource::~abstractResource() {
    this->removeIfUnused();
}
