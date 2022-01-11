#pragma once

#include <utility>
#include <unordered_map>
#include <resource/propertiesResource.h>

namespace chira {
    class TranslationFileResource : public PropertiesResource {
    public:
        TranslationFileResource(const std::string& identifier_, std::string language_)
            : PropertiesResource(identifier_)
            , language(std::move(language_)) {}
        void compile(const nlohmann::json& translations) override {
            this->strings = translations;
        }
        [[nodiscard]] const std::string& getLanguage() const;
        [[nodiscard]] bool hasTranslation(const std::string& key) const;
        /// Note: Currently nlohmann::json does not support string_view unfortunately.
        /// If this ever changes, please change this function to use it!
        [[nodiscard]] std::string getTranslation(const std::string& identifier) const;
        [[nodiscard]] std::unordered_map<std::string, std::string> getAllTranslations() const;
    private:
        std::string language;
        nlohmann::json strings;
    };
}
