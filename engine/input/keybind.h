#pragma once

#include <functional>

namespace chira {
    class keybind {
    public:
        // todo: allow keybinds to have multiple fire buttons
        keybind(int button, int action, const std::function<void()>& function, bool isMouse = false);
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
