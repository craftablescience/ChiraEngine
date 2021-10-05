#pragma once

#include "abstractWorld.h"
#include <functional>

namespace chira {
    /// Very useful for rapid iteration.
    /// Allows you to run arbitrary code.
    /// It's probably a bad idea to use this in production,
    /// convert your code to a hard world subclass when it works!
    class extensibleWorld : public abstractWorld {
    public:
        extensibleWorld(const std::function<void(double)>& preRenderFunction_, const std::function<void(double)>& postRenderFunction_, const std::function<void()>& deathFunction_) {
            this->preRenderFunction = preRenderFunction_;
            this->postRenderFunction = postRenderFunction_;
            this->deathFunction = deathFunction_;
        }
        void preRender(double delta) override  {
            this->preRenderFunction(delta);
        }
        void postRender(double delta) override {
            this->postRenderFunction(delta);
        }
        ~extensibleWorld() override {
            this->deathFunction();
        }
    private:
        std::function<void(double)> preRenderFunction = nullptr;
        std::function<void(double)> postRenderFunction = nullptr;
        std::function<void()> deathFunction = nullptr;
    };
}
