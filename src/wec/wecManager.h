#pragma once

#include <vector>
#include <memory>
#include "component/abstractComponent.h"
#include "entity/abstractEntity.h"
#include "world/abstractWorld.h"

class wecManager {
public:
    void addComponent(abstractComponent* component);
    void addEntity(abstractEntity* entity);
    void addWorld(abstractWorld* world);

    void preRender();
    void postRender();
    void stop();
private:
    std::vector<std::unique_ptr<abstractComponent>> components{};
    std::vector<std::unique_ptr<abstractEntity>> entities{};
    std::vector<std::unique_ptr<abstractWorld>> worlds{};
};
