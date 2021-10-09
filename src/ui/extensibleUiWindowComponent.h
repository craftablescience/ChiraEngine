#pragma once

#include "abstractUiWindowComponent.h"
#include <functional>

namespace chira {
    /// Very useful for rapid iteration.
    /// Allows you to run arbitrary code.
    /// It's definitely a bad idea to use this in production,
    /// convert your code to a hard subclass when it works!
    class extensibleUiWindowComponent : public abstractUiWindowComponent {
    public:
        extensibleUiWindowComponent(const std::string& title, bool startVisible, const std::function<void(double)>& drawFunc, const ImVec2& windowSize = ImVec2(0,0), bool enforceSize = false);
        void draw(double delta) final;
    private:
        std::function<void(double)> drawFunction;
    };
}
