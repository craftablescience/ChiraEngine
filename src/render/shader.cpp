#include "shader.h"
#include "glad/gl.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

shader::shader(const std::string& vertex, const std::string& fragment)
    : handleObject(), vert(GL_VERTEX_SHADER, vertex), frag(GL_FRAGMENT_SHADER, fragment) {}

shader::~shader() {
    if (this->handle != -1) glDeleteProgram(this->handle);
}

void shader::compile() {
    if (this->handle != -1) return;
    this->handle = glCreateProgram();
    vert.compile();
    glAttachShader(this->handle, vert.getHandle());
    frag.compile();
    glAttachShader(this->handle, frag.getHandle());
    glLinkProgram(this->handle);
#if DEBUG
    this->checkForCompilationErrors();
#endif
    vert.discard();
    frag.discard();
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

void shader::setUniform(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(this->handle, name.c_str()), (int) value);
}

void shader::setUniform(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(this->handle, name.c_str()), value);
}

void shader::setUniform(const std::string& name, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(this->handle, name.c_str()), value);
}

void shader::setUniform(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(this->handle, name.c_str()), value);
}

void shader::setUniform(const std::string& name, bool value1, bool value2) const {
    glUniform2i(glGetUniformLocation(this->handle, name.c_str()), (int) value1, (int) value2);
}

void shader::setUniform(const std::string& name, int value1, int value2) const {
    glUniform2i(glGetUniformLocation(this->handle, name.c_str()), value1, value2);
}

void shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2) const {
    glUniform2ui(glGetUniformLocation(this->handle, name.c_str()), value1, value2);
}

void shader::setUniform(const std::string& name, float value1, float value2) const {
    glUniform2f(glGetUniformLocation(this->handle, name.c_str()), value1, value2);
}

void shader::setUniform(const std::string& name, bool value1, bool value2, bool value3) const {
    glUniform3i(glGetUniformLocation(this->handle, name.c_str()), (int) value1, (int) value2, (int) value3);
}

void shader::setUniform(const std::string& name, int value1, int value2, int value3) const {
    glUniform3i(glGetUniformLocation(this->handle, name.c_str()), value1, value2, value3);
}

void shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3) const {
    glUniform3ui(glGetUniformLocation(this->handle, name.c_str()), value1, value2, value3);
}

void shader::setUniform(const std::string& name, float value1, float value2, float value3) const {
    glUniform3f(glGetUniformLocation(this->handle, name.c_str()), value1, value2, value3);
}

void shader::setUniform(const std::string& name, bool value1, bool value2, bool value3, bool value4) const {
    glUniform4i(glGetUniformLocation(this->handle, name.c_str()), (int) value1, (int) value2, (int) value3, (int) value4);
}

void shader::setUniform(const std::string& name, int value1, int value2, int value3, int value4) const {
    glUniform4i(glGetUniformLocation(this->handle, name.c_str()), value1, value2, value3, value4);
}

void shader::setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4) const {
    glUniform4ui(glGetUniformLocation(this->handle, name.c_str()), value1, value2, value3, value4);
}

void shader::setUniform(const std::string& name, float value1, float value2, float value3, float value4) const {
    glUniform4f(glGetUniformLocation(this->handle, name.c_str()), value1, value2, value3, value4);
}

void shader::setUniform(const std::string& name, glm::mat4* value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(*value));
}

void shader::discard() {
    if (this->handle != -1) glDeleteProgram(this->handle);
}
