#ifndef BASICGAMEENGINE_KEYBIND_H
#define BASICGAMEENGINE_KEYBIND_H


#include <functional>
#include "../core/engine.h"

class engine;

class keybind {
public:
    // todo: allow keybinds to have multiple fire buttons
    keybind(int button, int action, const std::function<void(engine*)>& function, bool sticky = false);
    [[nodiscard]] int getButton() const;
    [[nodiscard]] int getAction() const;
    [[nodiscard]] bool isSticky() const;
    [[nodiscard]] bool wasJustFired() const;
    void run(engine* engine);
    void unstick();
private:
    int button;
    int action;
    std::function<void(engine*)> fire;
    bool sticky;
    bool justFired;
};


#endif //BASICGAMEENGINE_KEYBIND_H
