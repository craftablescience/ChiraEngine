#include "shaderResource.h"

#include <iostream>

// todo: add #include preprocessing
shaderResource::shaderResource(const std::string& provider_, const std::string& name_, unsigned int type_) : abstractResource(provider_, name_), handleObject(), type(type_) {}

shaderResource::~shaderResource() {
    if (this->handle != -1) glDeleteShader(this->handle);
    this->removeIfUnused();
}

void shaderResource::checkForCompilationErrors() const {
    int success;
    char infoLog[512];
    glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(this->handle, 512, nullptr, infoLog);
        // todo: use new logger in feat/wec-conversion
        std::cerr << "Error: shader compilation failed. Type: " << this->type << std::endl;
        std::cerr << infoLog << std::endl;
    }
}

unsigned int shaderResource::getType() const {
    return this->type;
}

void shaderResource::addPreprocessorSymbol(const std::string& name, const std::string& value) {
    if (shaderResource::preprocessorSymbols.count(name) == 0) {
        shaderResource::preprocessorSymbols.insert(std::pair<std::string, std::string>{name, value});
    } else {
        shaderResource::preprocessorSymbols[name] = value;
    }
}

void shaderResource::setPreprocessorPrefix(const std::string& prefix) {
    shaderResource::preprocessorPrefix = prefix;
}

void shaderResource::setPreprocessorSuffix(const std::string& suffix) {
    shaderResource::preprocessorSuffix = suffix;
}
