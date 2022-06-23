#include "PropertiesResource.h"

#include <i18n/TranslationManager.h>
#include <utility/Logger.h>

using namespace chira;

void PropertiesResource::compile(const unsigned char buffer[], std::size_t bufferLength) {
    nlohmann::json props = nlohmann::json::parse("{}");
    try {
        props = nlohmann::json::parse(std::string{reinterpret_cast<const char*>(buffer), bufferLength});
    } catch (const nlohmann::json::exception&) {
        Logger::log(LOG_ERROR, "Properties Resource", TRF("error.properties_resource.invalid_json", this->identifier));
    }
    if (!hasProperty(props, "dependencies"))
        props["dependencies"] = nlohmann::json::parse("{}");
    if (!hasProperty(props, "properties"))
        props["properties"] = nlohmann::json::parse("{}");
    this->compile(props);
}

void PropertiesResource::logMissingProperty(const std::string& identifier, const std::string& key) {
    Logger::log(LOG_ERROR, "Properties Resource", TRF("warn.properties_resource.missing_property", identifier, key));
}
