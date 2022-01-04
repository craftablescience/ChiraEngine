#pragma once

#include <functional>

namespace chira {
    class Keybind {
    public:
        // todo: allow keybinds to have multiple fire buttons
        Keybind(int button, int action, const std::function<void()>& function, bool isMouse = false);
        [[nodiscard]] int getButton() const;
        [[nodiscard]] int getAction() const;
        [[nodiscard]] bool isMouse() const;
        void run();
    private:
        int button;
        int action;
        bool mouse;
        std::function<void()> fire;
    };
}
