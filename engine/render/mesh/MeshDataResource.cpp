#include "MeshDataResource.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>

#include <utility>

using namespace chira;

CHIRA_CREATE_LOG(MESHDATARESOURCE);

void MeshDataResource::compile(const nlohmann::json& properties) {
    Reflect::fromJSON(this, properties);
    if (!this->materialSetInCode) {
        this->material = CHIRA_GET_MATERIAL(this->materialType, this->materialPath);
    }
    this->appendMeshData(this->modelLoader, this->modelPath);
    this->setupForRendering();
}

void MeshDataResource::setDepthFunction(std::string depthFuncStr_) {
    this->depthFuncStr = std::move(depthFuncStr_);
    this->depthFunction = getMeshDepthFunctionFromString(this->depthFuncStr);
}

void MeshDataResource::setCullType(std::string cullTypeStr_) {
    this->cullTypeStr = std::move(cullTypeStr_);
    this->cullType = getMeshCullTypeFromString(this->cullTypeStr);
}
