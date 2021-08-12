#pragma once

#include <string>
#include <unordered_map>
#include "../utility/handleObject.h"

const std::string PREPROCESSOR_DEFAULT_PREFIX = "#";
const std::string PREPROCESSOR_DEFAULT_SUFFIX = "#";

class shaderFile : public handleObject {
public:
    shaderFile(unsigned int type, const std::string& source, bool isFilePath = true);
    virtual ~shaderFile();
    void compile() override;
    void discard() override;
    [[nodiscard]] unsigned int getType() const;
    static void addPreprocessorSymbol(const std::string& name, const std::string& value);
    static void setPreprocessorPrefix(const std::string& prefix);
    static void setPreprocessorSuffix(const std::string& suffix);
protected:
    unsigned int type;
    std::string source;
    static inline std::string preprocessorPrefix = PREPROCESSOR_DEFAULT_PREFIX;
    static inline std::string preprocessorSuffix = PREPROCESSOR_DEFAULT_SUFFIX;
    static inline std::unordered_map<std::string, std::string> preprocessorSymbols{};
private:
    static std::string loadSourceFromFile(const std::string& filepath);
    void checkForCompilationErrors() const;
};
