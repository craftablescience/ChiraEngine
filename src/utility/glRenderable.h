#ifndef BASICGAMEENGINE_GLRENDERABLE_H
#define BASICGAMEENGINE_GLRENDERABLE_H


#include "glCompilable.h"

class glRenderable : public glCompilable {
    virtual void compile() = 0;
    virtual void discard() = 0;
    virtual void render() = 0;
};


#endif //BASICGAMEENGINE_GLRENDERABLE_H
