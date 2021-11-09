#include "abstractResource.h"

#include "resourceManager.h"

using namespace chira;

abstractResource* abstractResource::copy() {
    this->incrementRefCount();
    return this;
}

void abstractResource::release() const {
    resourceManager::removeResource(this->identifier);
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
