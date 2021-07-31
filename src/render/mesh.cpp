#include "mesh.h"
#include "../core/engine.h"

mesh::mesh(abstractMeshLoader* loader, const std::string& filepath_, const std::string& material_, int depthFunc_, bool backfaceCulling_, int cullType_) : compilable(), model(1.0f), vertices(), indices() {
    this->depthFunc = depthFunc_;
    this->backfaceCulling = backfaceCulling_;
    this->cullType = cullType_;
    this->loader = loader;
    this->filepath = filepath_;
    this->material = material_;
    this->vboHandle = -1;
    this->vaoHandle = -1;
    this->eboHandle = -1;
}

mesh::~mesh() {
    glDeleteVertexArrays(1, &(this->vaoHandle));
    glDeleteBuffers(1, &(this->vboHandle));
    glDeleteBuffers(1, &(this->eboHandle));
}

void mesh::setMaterial(const std::string& material_) {
    this->material = material_;
}

abstractMaterial* mesh::getMaterial() {
    return engine::getMaterial(this->material);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, norm));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, col));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uvMap));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void mesh::discard() {
    glDeleteVertexArrays(1, &(this->vaoHandle));
    glDeleteBuffers(1, &(this->vboHandle));
    glDeleteBuffers(1, &(this->eboHandle));
}

void mesh::render() {
    engine::getMaterial(this->material)->use();
    engine::getMaterial(this->material)->getShader()->setUniform("m", &(this->model));

    glDepthFunc(this->depthFunc);
    if (this->backfaceCulling) {
        glEnable(GL_CULL_FACE);
        glCullFace(this->cullType);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glBindVertexArray(this->vaoHandle);
    glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, nullptr);
}
