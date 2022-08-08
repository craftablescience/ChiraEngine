#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <glad/gl.h>
#include <utility/HandleObject.h>
#include <resource/StringResource.h>

namespace chira {

constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_PREFIX = "#";
constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_SUFFIX = "#";

class ShaderResource : public StringResource, public HandleObject<int> {
public:
    ShaderResource(std::string identifier_, int type_);
    void compile(const byte buffer[], std::size_t bufferLength) override;
    ~ShaderResource() override;
    [[nodiscard]] unsigned int getType() const;
    static void addPreprocessorSymbol(const std::string& name, const std::string& value);
    static void setPreprocessorPrefix(const std::string& prefix);
    static void setPreprocessorSuffix(const std::string& suffix);
protected:
    using StringResource::getString;
private:
    unsigned int type;
    static inline std::string preprocessorPrefix = std::string{SHADER_PREPROCESSOR_DEFAULT_PREFIX}; // NOLINT(cert-err58-cpp)
    static inline std::string preprocessorSuffix = std::string{SHADER_PREPROCESSOR_DEFAULT_SUFFIX}; // NOLINT(cert-err58-cpp)
    static inline std::unordered_map<std::string, std::string> preprocessorSymbols;
    void checkForCompilationErrors() const;
};

} // namespace chira
