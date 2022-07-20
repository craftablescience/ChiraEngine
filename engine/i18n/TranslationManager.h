#pragma once

#include "TranslationFileResource.h"
#include <fmt/core.h>

namespace chira {

class TranslationManager {
public:
    static void setLanguage(const std::string& languageCode);
    static std::string_view getLanguage();
    static const std::unordered_map<std::string,std::string>& getCodeAndNamePairs();
    static std::string_view getLanguageNameFromCode(const std::string& code);
    static bool isValidCode(const std::string& code);
    static void addTranslationFile(const std::string& identifier);
    static void addUniversalFile(const std::string& identifier);
    static std::string getTranslation(const std::string& identifier);
    template<typename... Params> static std::string getTranslation(const std::string& identifier, Params... params) {
        return fmt::format(fmt::runtime(TranslationManager::getTranslation(identifier)), params...);
    }

private:
    static std::unordered_map<std::string,std::string> languageStrings;
    static std::string currentLanguage;
    static std::unordered_map<std::string, std::string> LANGUAGE_DEFINITIONS;
};

} // namespace chira

/// Get translation from identifier
#define TR(ID) chira::TranslationManager::getTranslation(ID)
/// Get translation from identifier as const char*
#define TRC(ID) TR(ID).c_str()
/// Get translation from identifier for use in fmt::format
#define TRF(ID, ...) chira::TranslationManager::getTranslation(ID, __VA_ARGS__)
