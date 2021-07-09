#ifndef BASICGAMEENGINE_SHADERBASE_H
#define BASICGAMEENGINE_SHADERBASE_H


#include <string>
#include <glad/gl.h>
#include <iostream>
#include "../utility/glObject.h"

class shaderBase : public glObject {
public:
    shaderBase(unsigned int type, const char* source, bool isFilePath = true);
    void compileShader();
    [[nodiscard]] unsigned int getType() const;
protected:
    unsigned int type;
    const char* source{};
private:
    bool compiled;
    static std::string loadSourceFromFile(const std::string& filepath);
    void checkForCompilationErrors() const;
};


#endif //BASICGAMEENGINE_SHADERBASE_H
