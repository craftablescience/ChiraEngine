#include "mesh.h"

#include "../resource/resourceManager.h"

mesh::mesh(const std::string& provider_, const std::string& name_, const std::shared_ptr<material>& material) : abstractMetaResource(provider_, name_), model(1.0f), vertices(), indices() {
    this->materialPtr = material;
}

mesh::~mesh() {
    if (this->compiled) {
        glDeleteVertexArrays(1, &(this->vaoHandle));
        glDeleteBuffers(1, &(this->vboHandle));
        glDeleteBuffers(1, &(this->eboHandle));
        this->compiled = false;
        this->vboHandle = this->eboHandle = this->vaoHandle = 0;
    }
}

void mesh::compile(const nlohmann::json& properties) {
    if (properties.contains("depthFunction")) {
        this->depthFunction = mesh::getGLDepthFuncFromString(properties["properties"]["depthFunc"]);
    }
    if (properties.contains("backfaceCulling")) {
        this->backfaceCulling = properties["properties"]["backfaceCulling"];
    }
    if (properties.contains("cullType")) {
        this->cullType = mesh::getGLCullTypeFromString(properties["properties"]["cullType"]);
    }

    mesh::getMeshLoader(properties["properties"]["loader"])->loadMesh(this->provider, properties["dependencies"]["model"], &this->vertices, &this->indices);

    glGenVertexArrays(1, &(this->vaoHandle));
    glGenBuffers(1, &(this->vboHandle));
    glGenBuffers(1, &(this->eboHandle));

    glBindVertexArray(this->vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (this->vertices.size() * sizeof(vertex)), &this->vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (this->indices.size() * sizeof(unsigned int)), &this->indices[0], GL_STATIC_DRAW);

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

    this->compiled = true;
}

void mesh::render() {
    this->materialPtr->use();
    std::shared_ptr<shader> s = this->materialPtr->getShader().lock();
    s->setUniform("m", &(this->model));

    glDepthFunc(this->depthFunction);
    if (this->backfaceCulling) {
        glEnable(GL_CULL_FACE);
        glCullFace(this->cullType);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glBindVertexArray(this->vaoHandle);
    glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, nullptr);
}

void mesh::addMeshLoader(const std::string& name, abstractMeshLoader* meshLoader) {
    mesh::meshLoaders[name] = std::unique_ptr<abstractMeshLoader>(meshLoader);
}

abstractMeshLoader* mesh::getMeshLoader(const std::string& name) {
    return mesh::meshLoaders[name].get();
}

int mesh::getGLDepthFuncFromString(const std::string& depthFunc) {
    if (depthFunc == "NEVER") {
        return GL_NEVER;
    } else if (depthFunc == "ALWAYS") {
        return GL_ALWAYS;
    } else if (depthFunc == "LESS") {
        return GL_LESS;
    } else if (depthFunc == "LEQUAL") {
        return GL_LEQUAL;
    } else if (depthFunc == "EQUAL") {
        return GL_EQUAL;
    } else if (depthFunc == "GEQUAL") {
        return GL_GEQUAL;
    } else if (depthFunc == "GREATER") {
        return GL_GREATER;
    } else if (depthFunc == "NOTEQUAL") {
        return GL_NOTEQUAL;
    }
    chiraLogger::log(WARN, "Mesh", "Invalid depth function value " + depthFunc);
    return GL_LEQUAL;
}

int mesh::getGLCullTypeFromString(const std::string& cullType) {
    if (cullType == "BACK") {
        return GL_BACK;
    } else if (cullType == "FRONT") {
        return GL_FRONT;
    } else if (cullType == "FRONT_AND_BACK" || cullType == "BACK_AND_FRONT") {
        return GL_FRONT_AND_BACK;
    }
    chiraLogger::log(WARN, "Mesh", "Invalid cull type value " + cullType);
    return GL_BACK;
}
