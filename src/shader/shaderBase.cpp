#include "shaderBase.h"

shaderBase::shaderBase(unsigned int type, const char* source, bool isFilePath) : glObject() {
    this->type = type;
    this->handle = glCreateShader(type);
    this->compiled = false;
    this->discarded = false;
}

void shaderBase::compileShader() {
    if (this->compiled) return;
    glShaderSource(this->handle, 1, &(this->source), nullptr);
    glCompileShader(this->handle);
#if DEBUG
    this->checkForCompilationErrors();
#endif
    this->compiled = true;
}

std::string shaderBase::loadSourceFromFile(const std::string &filepath) {
    return ""; // todo: load shaders from file
}

void shaderBase::checkForCompilationErrors() const {
    int success;
    char infoLog[512];
    glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(this->handle, 512, nullptr, infoLog);
        std::cerr << "Error: shader compilation failed. Type: " << this->type << "\nDetails: " << infoLog << std::endl;
    }
}

unsigned int shaderBase::getType() const {
    return this->type;
}
