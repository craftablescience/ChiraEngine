#include "translationFileResource.h"

using namespace chira;

const std::string& translationFileResource::getLanguage() {
    return this->language;
}

bool translationFileResource::hasTranslation(const std::string& key) {
    return this->strings.contains(key);
}

std::string translationFileResource::getTranslation(const std::string& identifier) {
    if (this->strings.contains(identifier)) {
        return this->strings[identifier];
    }
    return identifier + "#" + this->language;
}

std::unordered_map<std::string, std::string> translationFileResource::getAllTranslations() {
    return this->strings.get<std::unordered_map<std::string, std::string>>();
}
