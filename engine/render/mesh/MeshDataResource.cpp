#include "MeshDataResource.h"

#include <i18n/TranslationManager.h>
#include <utility/Logger.h>

using namespace chira;

void MeshDataResource::compile(const nlohmann::json& properties) {
    if (!getProperty(properties["properties"], "materialSetInCode", false)) {
        this->material = CHIRA_GET_MATERIAL(
                getProperty<std::string>(properties["dependencies"], "materialType", "MaterialTextured", true),
                getProperty<std::string>(properties["dependencies"], "material", "file://materials/unlitTextured.json", true));
    }
    this->depthFunction = MeshDataResource::getDepthFuncFromString(getProperty<std::string>(properties["properties"], "depthFunction", "LEQUAL"));
    this->backfaceCulling = getProperty(properties["properties"], "backfaceCulling", true);
    this->cullType = MeshDataResource::getCullTypeFromString(getProperty<std::string>(properties["properties"], "cullType", "BACK"));

    if (hasProperty(properties["dependencies"], "model") && Resource::hasResource(properties["dependencies"]["model"])) {
        this->appendMeshData(
                getProperty<std::string>(properties["properties"], "loader", "cmdl", true),
                properties["dependencies"]["model"]);
    } else {
        PropertiesResource::logMissingProperty(this->identifier, "model");
        this->appendMeshData("cmdl", "file://meshes/missing.cmdl");
    }
    this->setupForRendering();
}

int MeshDataResource::getDepthFuncFromString(std::string_view depthFunc) {
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

    Logger::log(LOG_WARNING, "Mesh", TRF("warn.mesh.invalid_depth_function", depthFunc));
    return GL_LEQUAL;
}

int MeshDataResource::getCullTypeFromString(std::string_view cullType) {
    if (cullType == "BACK")
        return GL_BACK;
    else if (cullType == "FRONT")
        return GL_FRONT;
    else if (cullType == "FRONT_AND_BACK" || cullType == "BACK_AND_FRONT")
        return GL_FRONT_AND_BACK;

    Logger::log(LOG_WARNING, "Mesh", TRF("warn.mesh.invalid_cull_type", cullType));
    return GL_BACK;
}
