#ifndef BASICGAMEENGINE_HANDLEOBJECT_H
#define BASICGAMEENGINE_HANDLEOBJECT_H


#include "compilable.h"

class handleObject : public compilable {
public:
    [[nodiscard]] int getHandle() const;
protected:
    int handle = -1;
};


#endif //BASICGAMEENGINE_HANDLEOBJECT_H
