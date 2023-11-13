#pragma once

#include <fmt/core.h>

#include "TranslationFileResource.h"

namespace chira {

class TranslationManager {
public:
    static const std::unordered_map<std::string,std::string>& getCodeAndNamePairs();
    static std::string_view getLanguageNameFromCode(const std::string& code);
    static bool isValidCode(const std::string& code);
    static void addTranslationFile(const std::string& path);
    static void addUniversalFile(const std::string& path);
    static std::string getTranslation(const std::string& identifier);
    template<typename... Params> static std::string getTranslation(const std::string& identifier, Params... params) {
        return fmt::format(fmt::runtime(TranslationManager::getTranslation(identifier)), params...);
    }
private:
    static inline std::unordered_map<std::string,std::string> languageStrings;
};

} // namespace chira

/// Get translation from identifier
#define TR(ID) chira::TranslationManager::getTranslation(ID)
/// Get translation from identifier as const char*
#define TRC(ID) TR(ID).c_str()
/// Get translation from identifier for use in fmt::format
#define TRF(ID, ...) chira::TranslationManager::getTranslation(ID, __VA_ARGS__)
