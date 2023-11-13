#pragma once

#include <utility>
#include <unordered_map>

#include <resource/JSONResource.h>

namespace chira {

class TranslationFileResource : public JSONResource {
public:
    TranslationFileResource(std::string path_, std::string language_)
        : JSONResource(std::move(path_))
        , language(std::move(language_)) {}

    void compile(const nlohmann::json& translations) override {
        this->strings = translations;
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
