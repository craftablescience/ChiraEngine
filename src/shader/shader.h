#ifndef BASICGAMEENGINE_SHADER_H
#define BASICGAMEENGINE_SHADER_H


#include "shaderFile.h"
#include "../utility/glObject.h"

class shader : public glObject {
public:
    shader(const std::string& vert, const std::string& frag);
    void compile();
    void use();
    void discard();
private:
    shaderFile vert;
    shaderFile frag;
    void checkForCompilationErrors() const;
};


#endif //BASICGAMEENGINE_SHADER_H
