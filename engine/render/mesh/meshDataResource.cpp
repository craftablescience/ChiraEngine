#include "meshDataResource.h"

#include <i18n/translationManager.h>
#include <utility/logger.h>

using namespace chira;

void MeshDataResource::compile(const nlohmann::json& properties) {
    if (!getPropertyOrDefault(properties["properties"], "materialSetInCode", false))
        this->material = CHIRA_GET_MATERIAL(properties["dependencies"]["materialType"], properties["dependencies"]["material"]);
    if (properties["properties"].contains("depthFunction"))
        this->depthFunction = MeshDataResource::getDepthFuncFromString(properties["properties"]["depthFunction"]);
    if (properties["properties"].contains("backfaceCulling"))
        this->backfaceCulling = properties["properties"]["backfaceCulling"];
    if (properties["properties"].contains("cullType"))
        this->cullType = MeshDataResource::getCullTypeFromString(properties["properties"]["cullType"]);

    this->setMeshData(properties["properties"]["loader"], properties["dependencies"]["model"], false);
    this->setupForRendering();
}

int MeshDataResource::getDepthFuncFromString(const std::string& depthFunc) {
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

    Logger::log(LogType::WARNING, "Mesh", TRF("warn.mesh.invalid_depth_function", depthFunc));
    return GL_LEQUAL;
}

int MeshDataResource::getCullTypeFromString(const std::string& cullType) {
    if (cullType == "BACK")
        return GL_BACK;
    else if (cullType == "FRONT")
        return GL_FRONT;
    else if (cullType == "FRONT_AND_BACK" || cullType == "BACK_AND_FRONT")
        return GL_FRONT_AND_BACK;

    Logger::log(LogType::WARNING, "Mesh", TRF("warn.mesh.invalid_cull_type", cullType));
    return GL_BACK;
}
