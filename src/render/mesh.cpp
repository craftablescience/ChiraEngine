#include "mesh.h"

mesh::mesh(meshLoader* loader, const std::string& filepath, int depthFunc) : glRenderable(), vertices(), indices() {
    this->depthFunc = depthFunc;
    this->loader = loader;
    this->filepath = filepath;
    this->vboHandle = -1;
    this->vaoHandle = -1;
    this->eboHandle = -1;
}

mesh::~mesh() {
    glDeleteVertexArrays(1, &(this->vaoHandle));
    glDeleteBuffers(1, &(this->vboHandle));
    glDeleteBuffers(1, &(this->eboHandle));
}

void mesh::compile() {
    this->loader->loadMesh(this->filepath, &this->vertices, &this->indices);

    glGenVertexArrays(1, &(this->vaoHandle));
    glGenBuffers(1, &(this->vboHandle));
    glGenBuffers(1, &(this->eboHandle));

    glBindVertexArray(this->vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(vertex), &this->vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void mesh::discard() {
    glDeleteVertexArrays(1, &(this->vaoHandle));
    glDeleteBuffers(1, &(this->vboHandle));
    glDeleteBuffers(1, &(this->eboHandle));
}

void mesh::render(shader* shader) {
    // todo: pass a "material" object instead of a shader
    shader->use();
    shader->setUniform("m", &(this->model));
    glDepthFunc(this->depthFunc);
    glBindVertexArray(this->vaoHandle);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}
