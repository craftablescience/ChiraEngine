#ifndef BASICGAMEENGINE_GLOBJECT_H
#define BASICGAMEENGINE_GLOBJECT_H


class glObject {
public:
    [[nodiscard]] int getHandle() const;
protected:
    int handle;
};


#endif //BASICGAMEENGINE_GLOBJECT_H
