#include "meshResource.h"

#include "../utility/math/matrix.h"
#include <fmt/core.h>
#include "../i18n/translationManager.h"
#include "../resource/resourceManager.h"

using namespace chira;

meshResource::meshResource(const std::string& identifier_, material* material) : propertiesResource(identifier_) {
    this->materialPtr = material;
}

meshResource::~meshResource() {
    glDeleteVertexArrays(1, &(this->vaoHandle));
    glDeleteBuffers(1, &(this->vboHandle));
    glDeleteBuffers(1, &(this->eboHandle));
    this->vboHandle = this->eboHandle = this->vaoHandle = 0;
}

void meshResource::compile(const nlohmann::json& properties) {
    if (properties["properties"].contains("depthFunction")) {
        this->depthFunction = meshResource::getGLDepthFuncFromString(properties["properties"]["depthFunction"]);
    }
    if (properties["properties"].contains("backfaceCulling")) {
        this->backfaceCulling = properties["properties"]["backfaceCulling"];
    }
    if (properties["properties"].contains("cullType")) {
        this->cullType = meshResource::getGLCullTypeFromString(properties["properties"]["cullType"]);
    }

    meshResource::getMeshLoader(properties["properties"]["loader"])->loadMesh(properties["dependencies"]["model"], &this->vertices, &this->indices);

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
}

void meshResource::release() const {
    this->materialPtr->release();
    abstractResource::release();
}

void meshResource::render(glm::vec3* position, glm::quat* rotation) {
    this->materialPtr->use();
    shader* s = this->materialPtr->getShader();
    glm::mat4 model = transformToMatrix(*position, *rotation);
    s->setUniform("m", &model);

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

void meshResource::addMeshLoader(const std::string& name, abstractMeshLoader* meshLoader) {
    meshResource::meshLoaders[name] = std::unique_ptr<abstractMeshLoader>(meshLoader);
}

abstractMeshLoader* meshResource::getMeshLoader(const std::string& name) {
    return meshResource::meshLoaders[name].get();
}

int meshResource::getGLDepthFuncFromString(const std::string& depthFunc) {
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
    logger::log(WARN, "Mesh", fmt::format(TR("warn.mesh.invalid_depth_function"), depthFunc));
    return GL_LEQUAL;
}

int meshResource::getGLCullTypeFromString(const std::string& cullType) {
    if (cullType == "BACK") {
        return GL_BACK;
    } else if (cullType == "FRONT") {
        return GL_FRONT;
    } else if (cullType == "FRONT_AND_BACK" || cullType == "BACK_AND_FRONT") {
        return GL_FRONT_AND_BACK;
    }
    logger::log(WARN, "Mesh", fmt::format(TR("warn.mesh.invalid_cull_type"), cullType));
    return GL_BACK;
}
