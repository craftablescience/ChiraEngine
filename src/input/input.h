#ifndef BASICGAMEENGINE_INPUT_H
#define BASICGAMEENGINE_INPUT_H


#include <list>
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

class input {
public:
    std::list<keybind> keybinds;
    void addKeybind(const keybind& keybind);
};


#endif //BASICGAMEENGINE_INPUT_H
