#include "TranslationFileResource.h"

using namespace chira;

std::string_view TranslationFileResource::getLanguage() const {
    return this->language;
}

bool TranslationFileResource::hasTranslation(const std::string& key) const {
    return this->strings.contains(key);
}

std::string TranslationFileResource::getTranslation(const std::string& key) const {
    if (this->strings.contains(key)) {
        return this->strings.at(key);
    }
    return key + "#" + this->language;
}

std::unordered_map<std::string, std::string> TranslationFileResource::getAllTranslations() const {
    return this->strings.get<std::unordered_map<std::string, std::string>>();
}
