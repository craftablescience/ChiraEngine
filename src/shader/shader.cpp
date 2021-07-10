#include "shader.h"
#include "glad/gl.h"
#include <iostream>

shader::shader(const std::string& vertex, const std::string& fragment)
    : glObject(), vert(GL_VERTEX_SHADER, vertex), frag(GL_FRAGMENT_SHADER, fragment) {
    this->handle = -1;
}

void shader::compile() {
    if (this->handle >= 0) return;
    this->handle = glCreateProgram();
    vert.compile();
    glAttachShader(this->handle, vert.getHandle());
    frag.compile();
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
        std::cerr << "Error: shader linking failed." << std::endl;
        std::cerr << infoLog << std::endl;
    }
}

void shader::use() {
    glUseProgram(this->handle);
}

void shader::discard() {
    glDeleteProgram(this->handle);
}
