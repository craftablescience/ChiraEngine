#ifndef BASICGAMEENGINE_INPUT_H
#define BASICGAMEENGINE_INPUT_H


#include <list>
#include "keybind.h"

class input {
public:
    std::list<keybind> keybinds;
    void addKeybind(const keybind& keybind);
};


#endif //BASICGAMEENGINE_INPUT_H
