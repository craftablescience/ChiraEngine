#include "translationFileResource.h"

using namespace chira;

const std::string& TranslationFileResource::getLanguage() const {
    return this->language;
}

bool TranslationFileResource::hasTranslation(const std::string& key) const {
    return this->strings.contains(key);
}

std::string TranslationFileResource::getTranslation(const std::string& identifier) const {
    if (this->strings.contains(identifier)) {
        return this->strings.at(identifier);
    }
    return identifier + "#" + this->language;
}

std::unordered_map<std::string, std::string> TranslationFileResource::getAllTranslations() const {
    return this->strings.get<std::unordered_map<std::string, std::string>>();
}
