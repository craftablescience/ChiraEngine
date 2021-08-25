#pragma once

#include "abstractEntity.h"
#include <functional>

/*
 * Very useful for rapid iteration.
 * Allows you to run arbitrary code within this component.
 * It's definitely a bad idea to use this in production,
 * convert your code to a hard component subclass when it works!
 */
class extensibleEntity : public abstractEntity {
public:
    void setInitFunction(const std::function<void()>& function);
    void setRenderFunction(const std::function<void(double)>& function);
    void setStopFunction(const std::function<void()>& function);
    void init() override;
    void render(double delta) override;
    void stop() override;
private:
    std::function<void()> initFunction = nullptr;
    std::function<void(double)> renderFunction = nullptr;
    std::function<void()> stopFunction = nullptr;
};
