#ifndef BASICGAMEENGINE_SHADER_H
#define BASICGAMEENGINE_SHADER_H


#include "shaderFile.h"
#include "../utility/glObject.h"

class shader : public glObject {
public:
    shader(shaderFile vert, shaderFile frag);
    void use();
private:
    void checkForCompilationErrors() const;
};


#endif //BASICGAMEENGINE_SHADER_H
