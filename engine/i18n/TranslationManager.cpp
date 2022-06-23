#include "TranslationManager.h"

#include <resource/Resource.h>
#include <utility/Logger.h>

using namespace chira;

std::unordered_map<std::string,std::string> TranslationManager::languageStrings{};
std::string TranslationManager::currentLanguage = "en"; // NOLINT(cert-err58-cpp)
std::unordered_map<std::string, std::string> TranslationManager::LANGUAGE_DEFINITIONS = { // NOLINT(cert-err58-cpp)
        {"en", "English"},
        {"jp", "Japanese"}
}; // todo: add all major language definitions

void TranslationManager::setLanguage(const std::string& languageCode) {
    TranslationManager::currentLanguage = languageCode;
}

const std::string& TranslationManager::getLanguage() {
    return TranslationManager::currentLanguage;
}

const std::unordered_map<std::string,std::string>& TranslationManager::getCodeAndNamePairs() {
    return TranslationManager::LANGUAGE_DEFINITIONS;
}

const std::string& TranslationManager::getLanguageNameFromCode(const std::string& code) {
    return TranslationManager::LANGUAGE_DEFINITIONS[code];
}

bool TranslationManager::isValidCode(const std::string& code) {
    return TranslationManager::LANGUAGE_DEFINITIONS.count(code) > 0;
}

void TranslationManager::addTranslationFile(const std::string& identifier) {
    auto file = Resource::getResource<TranslationFileResource>(identifier + "_" + TranslationManager::currentLanguage + ".json", TranslationManager::currentLanguage);
    for (const auto& [id, value] : file->getAllTranslations()) {
        TranslationManager::languageStrings[id] = value;
    }
}

void TranslationManager::addUniversalFile(const std::string& identifier) {
    auto file = Resource::getResource<TranslationFileResource>(identifier + "_" + "universal.json", "universal");
    for (const auto& [id, value] : file->getAllTranslations()) {
        TranslationManager::languageStrings[id] = value;
    }
}

std::string TranslationManager::getTranslation(const std::string& identifier) { // NOLINT(misc-no-recursion)
    if (TranslationManager::languageStrings.count(identifier) > 0)
        return TranslationManager::languageStrings[identifier];
    else if (TranslationManager::languageStrings.count("error.translation_manager.missing_translation") > 0)
        Logger::log(LOG_ERROR, "I18N", TRF("error.translation_manager.missing_translation", TranslationManager::getLanguageNameFromCode(TranslationManager::currentLanguage), identifier));
    else
        // Turns out if we're missing one string, we could be missing all of them! Just default to English
        Logger::log(LOG_ERROR, "I18N", fmt::format("Missing {} translation of \"{}\"", TranslationManager::getLanguageNameFromCode(TranslationManager::currentLanguage), identifier));
    // Fallback
    return identifier + "#" + TranslationManager::currentLanguage;
}
