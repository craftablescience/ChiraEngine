#include "fmt/core.h"
#include "translationManager.h"

#include "../resource/resourceManager.h"

void chira::translationManager::setLanguage(const std::string& languageCode) {
    translationManager::currentLanguage = languageCode;
}

const std::string& chira::translationManager::getLanguage() {
    return translationManager::currentLanguage;
}

const std::unordered_map<std::string,std::string>& chira::translationManager::getCodeAndNamePairs() {
    return translationManager::langPairs;
}

const std::string& chira::translationManager::getLanguageNameFromCode(const std::string& code) {
    return translationManager::langPairs[code];
}

bool chira::translationManager::isValidCode(const std::string& code) {
    return translationManager::langPairs.count(code) > 0;
}

void chira::translationManager::addTranslationFile(const std::string& name) {
    chira::translationManager::addTranslationFile("i18n", name);
}

void chira::translationManager::addTranslationFile(const std::string& path, const std::string& name) {
    auto file = resourceManager::getResourceWithoutCaching<translationFileResource>("file", path + "/" + name + "_" + chira::translationManager::currentLanguage + ".json", chira::translationManager::currentLanguage);
    for (const auto& [identifier, value] : file->getAllTranslations()) {
        chira::translationManager::languageStrings[identifier] = value;
    }
}

void chira::translationManager::addUniversalFile(const std::string& name) {
    chira::translationManager::addUniversalFile("i18n", name);
}

void chira::translationManager::addUniversalFile(const std::string& path, const std::string& name) {
    auto file = resourceManager::getResourceWithoutCaching<translationFileResource>("file", path + "/" + name + "_" + "universal.json", "universal");
    for (const auto& [identifier, value] : file->getAllTranslations()) {
        chira::translationManager::languageStrings[identifier] = value;
    }
}

std::string chira::translationManager::getTranslation(const std::string& identifier) {
    if (chira::translationManager::languageStrings.count(identifier) > 0) {
        return chira::translationManager::languageStrings[identifier];
    }
    chira::logger::log(ERR, "I18N", fmt::format(TR("error.translation_manager.missing_translation"), chira::translationManager::getLanguageNameFromCode(chira::translationManager::currentLanguage), identifier));
    return identifier + "#" + chira::translationManager::currentLanguage;
}
