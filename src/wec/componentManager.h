#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "abstractComponent.h"
#include "abstractEntity.h"
#include "abstractWorld.h"

class componentManager {
public:
    void addComponent(const std::string& name, abstractComponent* component);
    void addEntity(const std::string& name, abstractEntity* entity);
    void addWorld(const std::string& name, abstractWorld* world);
    abstractComponent* getComponent(const std::string& name);
    abstractEntity* getEntity(const std::string& name);
    abstractWorld* getWorld(const std::string& name);
    void preRender(double delta);
    void render(double delta);
    void postRender(double delta);
    void stop();
private:
    std::unordered_map<std::string,std::unique_ptr<abstractComponent>> components{};
    std::unordered_map<std::string,std::unique_ptr<abstractEntity>> entities{};
    std::unordered_map<std::string,std::unique_ptr<abstractWorld>> worlds{};
};
