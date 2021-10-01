#pragma once

#include "abstractEntity.h"
#include <functional>

/*
 * Very useful for rapid iteration.
 * Allows you to run arbitrary code within this component.
 * It's probably a bad idea to use this in production,
 * convert your code to a hard component subclass when it works!
 */
class extensibleEntity : public abstractEntity {
public:
    void setInitFunction(const std::function<void()>& function);
    void setPreRenderFunction(const std::function<void(double)>& function);
    void setPostRenderFunction(const std::function<void(double)>& function);
    void setStopFunction(const std::function<void()>& function);

    void init() override;
    void preRender(double delta) override;
    void postRender(double delta) override;
    void stop() override;
private:
    std::function<void()> initFunction = nullptr;
    std::function<void(double)> preRenderFunction = nullptr;
    std::function<void(double)> postRenderFunction = nullptr;
    std::function<void()> stopFunction = nullptr;
};
