#include "shaderFile.h"

shaderFile::shaderFile(unsigned int type, const std::string& source, bool isFilePath) : glObject() {
    this->type = type;
    this->handle = glCreateShader(type);
    this->compiled = false;
    if (!isFilePath) {
        this->source = source.c_str();
    } else {
        this->source = shaderFile::loadSourceFromFile(source).c_str();
    }
}

void shaderFile::compileShader() {
    if (this->compiled) return;
    glShaderSource(this->handle, 1, &(this->source), nullptr);
    glCompileShader(this->handle);
#if DEBUG
    this->checkForCompilationErrors();
#endif
    this->compiled = true;
}

std::string shaderFile::loadSourceFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
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
        std::cerr << "Error: shader compilation failed. Type: " << this->type << "\nDetails: " << infoLog << std::endl;
    }
}

unsigned int shaderFile::getType() const {
    return this->type;
}
