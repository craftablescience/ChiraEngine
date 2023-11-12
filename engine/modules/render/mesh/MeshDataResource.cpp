#include "MeshDataResource.h"

#include <core/debug/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(MESHDATARESOURCE);

void MeshDataResource::compile(const std::byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    if (!this->materialSetInCode) {
        this->material = CHIRA_GET_MATERIAL(this->materialType, this->materialPath);
    }
    this->appendMeshData(this->modelLoader, this->modelPath);
    this->setupForRendering();
}
