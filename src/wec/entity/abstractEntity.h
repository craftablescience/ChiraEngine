#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "../component/abstractComponent.h"

class abstractEntity {
public:
    virtual void init() = 0;
    virtual void render(double delta) = 0;
    virtual void stop() = 0;
    void addComponent(const std::string& name, abstractComponent* component);
    abstractComponent* getComponent(const std::string& name);
protected:
    std::unordered_map<std::string, std::unique_ptr<abstractComponent>> components;
};
