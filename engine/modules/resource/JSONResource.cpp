#include "JSONResource.h"

#include <core/debug/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(JSONRESOURCE);

void JSONResource::compile(const std::byte buffer[], std::size_t bufferLength) {
    nlohmann::json props;
    try {
        props = nlohmann::json::parse(std::string{reinterpret_cast<const char*>(buffer), bufferLength});
    } catch (const nlohmann::json::exception&) {
        LOG_JSONRESOURCE.error(TRF("error.properties_resource.invalid_json", this->path));
    }
    this->compile(props);
}
