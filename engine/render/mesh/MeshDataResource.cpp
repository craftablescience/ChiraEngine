#include "MeshDataResource.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>

#include <utility>

using namespace chira;

CHIRA_CREATE_LOG(MESHDATARESOURCE);

void MeshDataResource::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);
    if (!this->materialSetInCode) {
        this->material = CHIRA_GET_MATERIAL(this->materialType, this->materialPath);
    }
    this->appendMeshData(this->modelLoader, this->modelPath);
    this->setupForRendering();
}

std::string MeshDataResource::getDepthFunctionString() const {
    return this->depthFuncStr;
}

std::string MeshDataResource::getCullTypeString() const {
    return this->cullTypeStr;
}

void MeshDataResource::setDepthFunction(std::string depthFuncStr_) {
    this->depthFuncStr = std::move(depthFuncStr_);
    this->depthFunction = MeshDataResource::getDepthFunctionFromString(this->depthFuncStr);
}

void MeshDataResource::setCullType(std::string cullTypeStr_) {
    this->cullTypeStr = std::move(cullTypeStr_);
    this->cullType = MeshDataResource::getCullTypeFromString(this->cullTypeStr);
}

int MeshDataResource::getDepthFunctionFromString(const std::string& depthFunc) {
    if (depthFunc == "NEVER")
        return GL_NEVER;
    else if (depthFunc == "ALWAYS")
        return GL_ALWAYS;
    else if (depthFunc == "LESS")
        return GL_LESS;
    else if (depthFunc == "LEQUAL")
        return GL_LEQUAL;
    else if (depthFunc == "EQUAL")
        return GL_EQUAL;
    else if (depthFunc == "GEQUAL")
        return GL_GEQUAL;
    else if (depthFunc == "GREATER")
        return GL_GREATER;
    else if (depthFunc == "NOTEQUAL")
        return GL_NOTEQUAL;

    LOG_MESHDATARESOURCE.warning(TRF("warn.mesh.invalid_depth_function", depthFunc));
    return GL_LEQUAL;
}

int MeshDataResource::getCullTypeFromString(const std::string& cullType) {
    if (cullType == "BACK")
        return GL_BACK;
    else if (cullType == "FRONT")
        return GL_FRONT;
    else if (cullType == "FRONT_AND_BACK" || cullType == "BACK_AND_FRONT")
        return GL_FRONT_AND_BACK;

    LOG_MESHDATARESOURCE.warning(TRF("warn.mesh.invalid_cull_type", cullType));
    return GL_BACK;
}
