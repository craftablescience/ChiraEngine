#include "abstractResource.h"

#include "resourceManager.h"

void abstractResource::release() const {
    resourceManager::removeResource(this->provider, this->name);
}

void abstractResource::incrementRefCount() {
    this->refCount++;
}

void abstractResource::decrementRefCount() {
    this->refCount--;
}

unsigned int abstractResource::getRefCount() const {
    return this->refCount;
}
