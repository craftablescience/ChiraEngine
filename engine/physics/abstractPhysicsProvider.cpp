#include "abstractPhysicsProvider.h"

#include <i18n/translationManager.h>

using namespace chira;

std::unique_ptr<AbstractPhysicsProvider> AbstractPhysicsProvider::physicsProvider = nullptr;

AbstractPhysicsProvider* AbstractPhysicsProvider::getPhysicsProvider() {
    if (!AbstractPhysicsProvider::physicsProvider)
        Logger::log(LogType::WARNING, "Physics Provider", TRF("error.engine.invalid_access", "physics provider", "AbstractPhysicsProvider::setPhysicsProvider"));
    return AbstractPhysicsProvider::physicsProvider.get();
}

void AbstractPhysicsProvider::setPhysicsProvider(AbstractPhysicsProvider* newPhysicsProvider) {
    AbstractPhysicsProvider::physicsProvider.reset(newPhysicsProvider);
}
