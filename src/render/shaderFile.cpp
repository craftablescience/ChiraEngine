#include "shaderFile.h"
#include <glad/gl.h>
#include <iostream>
#include <fstream>
#include "../core/virtualFileSystem.h"

// todo: add variable preprocessing and #include preprocessing

shaderFile::shaderFile(unsigned int type, const std::string& source, bool isFilePath) : handleObject() {
    this->type = type;
    if (!isFilePath) {
        this->source = source;
    } else {
        this->source = shaderFile::loadSourceFromFile(source);
    }
}

shaderFile::~shaderFile() {
    if (this->handle != -1) glDeleteShader(this->handle);
}

void shaderFile::compile() {
    if (this->handle != -1) return;
    this->handle = glCreateShader(type);
    char* code = &this->source[0];
    glShaderSource(this->handle, 1, &code, nullptr);
    glCompileShader(this->handle);
#if DEBUG
    this->checkForCompilationErrors();
#endif
}

void shaderFile::discard() {
    if (this->handle != -1) glDeleteShader(this->handle);
}

std::string shaderFile::loadSourceFromFile(const std::string& filepath) {
    std::ifstream file(virtualFileSystem::getShaderPath(filepath));
    if (!file.is_open()) {
        std::cerr << "Error: shader file at " << filepath << " unreadable!" << std::endl;
        return "";
    } else {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        std::string data = buffer.str();
        return data;
    }
}

void shaderFile::checkForCompilationErrors() const {
    int success;
    char infoLog[512];
    glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(this->handle, 512, nullptr, infoLog);
        std::cerr << "Error: shader compilation failed. Type: " << this->type << std::endl;
        std::cerr << infoLog << std::endl;
    }
}

unsigned int shaderFile::getType() const {
    return this->type;
}
