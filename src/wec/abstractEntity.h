#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "abstractComponent.h"

class abstractEntity : public abstractComponent {
public:
    virtual void init() = 0;
    virtual void preRender(double delta) = 0;
    virtual void postRender(double delta) = 0;
    void addComponent(const std::string& name, abstractComponent* component);
    abstractComponent* getComponent(const std::string& name);
protected:
    std::unordered_map<std::string, abstractComponent*> components;
};
