#pragma once

#include "translationFileResource.h"
#include <fmt/core.h>

namespace chira {
    class TranslationManager {
    public:
        static void setLanguage(const std::string& languageCode);
        static const std::string& getLanguage();
        static const std::unordered_map<std::string,std::string>& getCodeAndNamePairs();
        static const std::string& getLanguageNameFromCode(const std::string& code);
        static bool isValidCode(const std::string& code);
        static void addTranslationFile(const std::string& identifier);
        static void addUniversalFile(const std::string& identifier);
        static std::string getTranslation(const std::string& identifier);
    private:
        static std::unordered_map<std::string,std::string> languageStrings;
        static std::string currentLanguage;
        static std::unordered_map<std::string, std::string> LANGUAGE_DEFINITIONS;
    };
}

/// Just a handy shortcut, since this function call can be quite long.
#define TR(ID) chira::TranslationManager::getTranslation(ID)
#define TRC(ID) TR(ID).c_str()
