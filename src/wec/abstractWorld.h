#pragma once

#include "abstractEntity.h"

class abstractWorld {
public:
    explicit abstractWorld(std::string name_);
    virtual void init() = 0;
    virtual void preRender(double delta) = 0;
    virtual void postRender(double delta) = 0;
    virtual void stop() = 0;
    const std::string& getName();
    void addEntity(const std::string& name_, abstractEntity* component);
    abstractEntity* getEntity(const std::string& name_);
    void removeEntity(const std::string& name_);
private:
    std::string name;
};
