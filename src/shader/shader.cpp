#include "shader.h"

shader::shader(shaderFile vert, shaderFile frag) : glObject() {
    this->handle = glCreateProgram();
    vert.compileShader();
    glAttachShader(this->handle, vert.getHandle());
    frag.compileShader();
    glAttachShader(this->handle, frag.getHandle());
    glLinkProgram(this->handle);
#if DEBUG
    this->checkForCompilationErrors();
#endif
    glDeleteShader(vert.getHandle());
    glDeleteShader(frag.getHandle());
}

void shader::checkForCompilationErrors() const {
    int success;
    char infoLog[512];
    glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->handle, 512, nullptr, infoLog);
        std::cerr << "Error: shader linking failed.\nDetails: " << infoLog << std::endl;
    }
}

void shader::use() {
    glUseProgram(this->handle);
}
