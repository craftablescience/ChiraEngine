#include "PropertiesResource.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

void PropertiesResource::compile(const unsigned char buffer[], std::size_t bufferLength) {
    nlohmann::json props = nlohmann::json::parse("{}");
    try {
        props = nlohmann::json::parse(std::string{reinterpret_cast<const char*>(buffer), bufferLength});
    } catch (const nlohmann::json::exception&) {
        Logger::log(LogType::LOG_ERROR, "Properties Resource", TRF("error.properties_resource.invalid_json", this->identifier));
    }
    if (!props.contains("dependencies"))
        props["dependencies"] = nlohmann::json::parse("{}");
    if (!props.contains("properties"))
        props["properties"] = nlohmann::json::parse("{}");
    this->compile(props);
}

void PropertiesResource::logMissingProperty(std::string_view identifier, std::string_view key) {
    Logger::log(LogType::LOG_ERROR, "Properties Resource", TRF("warn.properties_resource.missing_property", identifier, key));
}
