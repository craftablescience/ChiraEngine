#pragma once

#include <utility>

#include "../resource/propertiesResource.h"

namespace chira {
    class translationFileResource : public propertiesResource {
    public:
        translationFileResource(const std::string& identifier_, std::string language_) :
                propertiesResource(identifier_), language(std::move(language_)) {}
        void compile(const nlohmann::json& translations) override {
            this->strings = translations;
        }
        translationFileResource* copy() override {
            this->incrementRefCount();
            return this;
        }
        const std::string& getLanguage();
        bool hasTranslation(const std::string& key);
        /// Note: Currently nlohmann::json does not support string_view unfortunately.
        /// If this ever changes, please change this function to use it!
        std::string getTranslation(const std::string& identifier);
        std::unordered_map<std::string,std::string> getAllTranslations();
    private:
        std::string language;
        nlohmann::json strings;
    };
}
