#include "translationManager.h"

#include <resource/resource.h>
#include <utility/logger.h>

using namespace chira;

std::unordered_map<std::string,std::string> translationManager::languageStrings{};
std::string translationManager::currentLanguage = "en"; // NOLINT(cert-err58-cpp)
std::unordered_map<std::string, std::string> translationManager::LANGUAGE_DEFINITIONS = { // NOLINT(cert-err58-cpp)
        {"en", "English"},
        {"jp", "Japanese"}
};

void translationManager::setLanguage(const std::string& languageCode) {
    translationManager::currentLanguage = languageCode;
}

const std::string& translationManager::getLanguage() {
    return translationManager::currentLanguage;
}

const std::unordered_map<std::string,std::string>& translationManager::getCodeAndNamePairs() {
    return translationManager::LANGUAGE_DEFINITIONS;
}

const std::string& translationManager::getLanguageNameFromCode(const std::string& code) {
    return translationManager::LANGUAGE_DEFINITIONS[code];
}

bool translationManager::isValidCode(const std::string& code) {
    return translationManager::LANGUAGE_DEFINITIONS.count(code) > 0;
}

void translationManager::addTranslationFile(const std::string& identifier) {
    auto file = resource::getResource<translationFileResource>(identifier + "_" + translationManager::currentLanguage + ".json", translationManager::currentLanguage);
    for (const auto& [id, value] : file->getAllTranslations()) {
        translationManager::languageStrings[id] = value;
    }
}

void translationManager::addUniversalFile(const std::string& identifier) {
    auto file = resource::getResource<translationFileResource>(identifier + "_" + "universal.json", "universal");
    for (const auto& [id, value] : file->getAllTranslations()) {
        translationManager::languageStrings[id] = value;
    }
}

std::string translationManager::getTranslation(const std::string& identifier) { // NOLINT(misc-no-recursion)
    if (translationManager::languageStrings.count(identifier) > 0) {
        return translationManager::languageStrings[identifier];
    }
    if (translationManager::languageStrings.count("error.translation_manager.missing_translation") > 0) {
        logger::log(ERR, "I18N", fmt::format(TR("error.translation_manager.missing_translation"), translationManager::getLanguageNameFromCode(translationManager::currentLanguage), identifier));
    } else {
        // Turns out if we're missing one string, we could be missing all of them! Just default to English
        logger::log(ERR, "I18N", fmt::format("Missing {} translation of \"{}\"", translationManager::getLanguageNameFromCode(translationManager::currentLanguage), identifier));
    }
    return identifier + "#" + translationManager::currentLanguage;
}
