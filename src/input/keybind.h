#ifndef BASICGAMEENGINE_KEYBIND_H
#define BASICGAMEENGINE_KEYBIND_H


#include <functional>
#include "../core/engine.h"

class engine;

class keybind {
public:
    // todo: allow keybinds to have multiple fire buttons
    keybind(int button, int action, const std::function<void(engine*)>& function, bool isMouse = false);
    [[nodiscard]] int getButton() const;
    [[nodiscard]] int getAction() const;
    [[nodiscard]] bool isMouse() const;
    void run(engine* engine);
private:
    int button;
    int action;
    bool mouse;
    std::function<void(engine*)> fire;
};


#endif //BASICGAMEENGINE_KEYBIND_H
