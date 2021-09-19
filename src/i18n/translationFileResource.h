#pragma once

#include <utility>

#include "../resource/propertiesResource.h"

class translationFileResource : public propertiesResource {
public:
    translationFileResource(const std::string& provider_, const std::string& name_, std::string language_) :
            propertiesResource(provider_, name_), language(std::move(language_)) {}
    void compile(const nlohmann::json& translations) override {
        this->strings = translations;
    }
    const std::string& getLanguage();
    bool hasTranslation(const std::string& key);
    /*
     * NOTE: Currently nlohmann::json does not support string_view unfortunately.
     * If this ever changes, please change this function to use it!
     */
    std::string getTranslation(const std::string& identifier);
    std::unordered_map<std::string,std::string> getAllTranslations();
private:
    std::string language;
    nlohmann::json strings;
};
