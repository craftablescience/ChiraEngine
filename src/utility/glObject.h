#ifndef BASICGAMEENGINE_GLOBJECT_H
#define BASICGAMEENGINE_GLOBJECT_H


#include "glCompilable.h"

class glObject : public glCompilable {
public:
    [[nodiscard]] int getHandle() const;
protected:
    int handle = -1;
};


#endif //BASICGAMEENGINE_GLOBJECT_H
