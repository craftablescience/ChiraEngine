#ifndef BASICGAMEENGINE_KEYBIND_H
#define BASICGAMEENGINE_KEYBIND_H


#include <functional>
#include "../core/engine.h"

class engine;

class keybind {
public:
    // todo: allow keybinds to have multiple fire buttons
    keybind(int button, int action, const std::function<void(engine*)>& function);
    [[nodiscard]] int getButton() const;
    [[nodiscard]] int getAction() const;
    void run(engine* engine);
private:
    int button;
    int action;
    std::function<void(engine*)> fire;
};


#endif //BASICGAMEENGINE_KEYBIND_H
