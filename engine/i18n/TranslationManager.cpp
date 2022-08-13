#include "TranslationManager.h"

#include <config/ConEntry.h>
#include <core/Logger.h>
#include <resource/Resource.h>

using namespace chira;

CHIRA_CREATE_LOG(I18N);

[[maybe_unused]] // todo: set language based on system language
static ConVar ui_language{"ui_language", std::string{"en"}, "The language code used to get translated strings.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

const std::unordered_map<std::string,std::string>& TranslationManager::getCodeAndNamePairs() {
    static std::unordered_map<std::string, std::string> languageDefinitions {
            {"en", "English"},
            {"jp", "Japanese"},
    }; // todo: add all major language definitions
    return languageDefinitions;
}

std::string_view TranslationManager::getLanguageNameFromCode(const std::string& code) {
    return TranslationManager::getCodeAndNamePairs().at(code);
}

bool TranslationManager::isValidCode(const std::string& code) {
    return TranslationManager::getCodeAndNamePairs().count(code) > 0;
}

void TranslationManager::addTranslationFile(const std::string& identifier) {
    auto file = Resource::getResource<TranslationFileResource>(identifier + "_" + ui_language.getValue<std::string>() + ".json", ui_language.getValue<std::string>());
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
        LOG_I18N.error(TRF("error.translation_manager.missing_translation", TranslationManager::getLanguageNameFromCode(ui_language.getValue<std::string>()), identifier));
    else
        // Turns out if we're missing one string, we could be missing all of them! Just default to English
        LOG_I18N.error(fmt::format("Missing {} translation of \"{}\"", TranslationManager::getLanguageNameFromCode(ui_language.getValue<std::string>()), identifier));
    // Fallback
    return identifier + "#" + ui_language.getValue<std::string>();
}
