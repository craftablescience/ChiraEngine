#pragma once

#include <utility>
#include <unordered_map>
#include <resource/PropertiesResource.h>

namespace chira {

class TranslationFileResource : public PropertiesResource {
public:
    TranslationFileResource(std::string identifier_, std::string language_)
        : PropertiesResource(std::move(identifier_))
        , language(std::move(language_)) {}
    void compile(const nlohmann::json& translations) override {
        this->strings = translations;
        this->strings.erase("properties");
        this->strings.erase("dependencies");
    }
    [[nodiscard]] std::string_view getLanguage() const;
    [[nodiscard]] bool hasTranslation(const std::string& key) const;
    /// Note: Currently nlohmann::json does not support string_view unfortunately.
    /// If this ever changes, please change this function to use it!
    [[nodiscard]] std::string getTranslation(const std::string& key) const;
    [[nodiscard]] std::unordered_map<std::string, std::string> getAllTranslations() const;
private:
    std::string language;
    nlohmann::json strings;
};

} // namespace chira
