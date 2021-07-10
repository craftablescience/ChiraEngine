#ifndef BASICGAMEENGINE_SHADERFILE_H
#define BASICGAMEENGINE_SHADERFILE_H


#include <string>
#include <glad/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../utility/glObject.h"

class shaderFile : public glObject {
public:
    shaderFile(unsigned int type, const std::string& source, bool isFilePath = true);
    void compile();
    [[nodiscard]] unsigned int getType() const;
protected:
    unsigned int type;
    const char* source{};
private:
    static std::string loadSourceFromFile(const std::string& filepath);
    void checkForCompilationErrors() const;
};


#endif //BASICGAMEENGINE_SHADERFILE_H
