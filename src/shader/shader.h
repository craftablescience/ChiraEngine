#ifndef BASICGAMEENGINE_SHADER_H
#define BASICGAMEENGINE_SHADER_H


#include "shaderBase.h"
#include "../utility/glObject.h"

class shader : public glObject {
public:
    shader(shaderBase vert, shaderBase frag);
    void use();
private:
    void checkForCompilationErrors() const;
};


#endif //BASICGAMEENGINE_SHADER_H
