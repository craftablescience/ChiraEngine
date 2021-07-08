#ifndef BASICGAMEENGINE_INPUT_H
#define BASICGAMEENGINE_INPUT_H


#include <list>

class keybind {
    typedef void (*funcptr)();
public:
    keybind(int button, funcptr function);
    [[nodiscard]] int getButton() const {
        return this->button;
    }
    void fire();
private:
    int button;
    funcptr function;
};

class input {
public:
    input();
    explicit input(std::list<keybind> &keybinds);
    void addKeybind(keybind keybind);
private:
    std::list<keybind> keybinds;
};


#endif //BASICGAMEENGINE_INPUT_H
