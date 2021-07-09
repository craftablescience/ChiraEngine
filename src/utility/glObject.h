#ifndef BASICGAMEENGINE_GLOBJECT_H
#define BASICGAMEENGINE_GLOBJECT_H


class glObject {
public:
    [[nodiscard]] unsigned int getHandle() const;
protected:
    unsigned int handle;
};


#endif //BASICGAMEENGINE_GLOBJECT_H
