#pragma once

#include "../entity/abstractEntity.h"

class abstractWorld {
public:
    virtual void init() = 0;
    virtual void preRender(double delta) = 0;
    virtual void postRender(double delta) = 0;
    virtual void stop() = 0;
    void addEntity(const std::string& name, abstractEntity* component);
    abstractEntity* getEntity(const std::string& name);
protected:
    std::unordered_map<std::string, std::unique_ptr<abstractEntity>> entities;
};
