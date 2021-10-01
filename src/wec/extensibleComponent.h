#pragma once

#include "abstractComponent.h"
#include <functional>

/*
 * Very useful for rapid iteration.
 * Allows you to run arbitrary code within this component.
 * It's definitely a bad idea to use this in production,
 * convert your code to a hard component subclass when it works!
 */
class extensibleComponent : public abstractComponent {
public:
    void setRenderFunction(const std::function<void(double)>& function) {
        this->renderFunction = function;
    }
    void setStopFunction(const std::function<void()>& function) {
        this->stopFunction = function;
    }
    void render(double delta) override {
        this->renderFunction(delta);
    }
    void stop() override {
        this->stopFunction();
    }
private:
    std::function<void(double)> renderFunction = nullptr;
    std::function<void()> stopFunction = nullptr;
};
