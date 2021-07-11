#include "mesh.h"
#include "glad/gl.h"

mesh::mesh(meshLoader* loader, const std::string& filepath) : vertices(), indices() {
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

    unsigned int vertexLength = (this->vertices.size() / 3) * 8;
    float verticesGL[vertexLength];
    unsigned int i = 0;
    for (vertex v : vertices) {
        verticesGL[i++] = v.x;
        verticesGL[i++] = v.y;
        if (v.is3D()) {
            verticesGL[i++] = v.z;
        }
    }
    unsigned int indexLength = this->indices.size();
    unsigned int indicesGL[indexLength];
    i = 0;
    for (unsigned int index : this->indices) {
        indicesGL[i++] = index;
    }

    glGenVertexArrays(1, &(this->vaoHandle));
    glGenBuffers(1, &(this->vboHandle));
    glGenBuffers(1, &(this->eboHandle));

    glBindVertexArray(this->vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGL), verticesGL, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesGL), indicesGL, GL_STATIC_DRAW);

    const int vertexArrayLength = 8;
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexArrayLength * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexArrayLength * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexArrayLength * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void mesh::discard() {
    glDeleteVertexArrays(1, &(this->vaoHandle));
    glDeleteBuffers(1, &(this->vboHandle));
    glDeleteBuffers(1, &(this->eboHandle));
}

void mesh::render() {
    glBindVertexArray(this->vaoHandle);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
