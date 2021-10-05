#pragma once

#include "abstractComponent.h"
#include <functional>

namespace chira {
    /// Very useful for rapid iteration.
    /// Allows you to run arbitrary code.
    /// It's definitely a bad idea to use this in production,
    /// convert your code to a hard component subclass when it works!
    class extensibleComponent : public abstractComponent {
    public:
        extensibleComponent(const std::function<void(double)>& renderFunction_, const std::function<void()>& deathFunction_) {
            this->renderFunction = renderFunction_;
            this->deathFunction = deathFunction_;
        }
        void render(double delta) override {
            this->renderFunction(delta);
        }
        ~extensibleComponent() override {
            this->deathFunction();
        }
    private:
        std::function<void(double)> renderFunction = nullptr;
        std::function<void()> deathFunction = nullptr;
    };
}
