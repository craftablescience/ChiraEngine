#ifndef BASICGAMEENGINE_KEYBIND_H
#define BASICGAMEENGINE_KEYBIND_H


#include <functional>

class keybind {
public:
    std::function<void ()> fire;
    keybind(int button, int action, const std::function<void ()>& function);
    [[nodiscard]] int getButton() const;
    [[nodiscard]] int getAction() const;
private:
    int button;
    int action;
};


#endif //BASICGAMEENGINE_KEYBIND_H
