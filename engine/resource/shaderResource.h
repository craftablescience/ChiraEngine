#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <glad/gl.h>
#include <utility/handleObject.h>
#include <resource/stringResource.h>

namespace chira {
    constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_PREFIX = "#";
    constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_SUFFIX = "#";

    class ShaderResource : public StringResource, public HandleObject<int> {
    public:
        ShaderResource(const std::string& identifier_, int type_);
        void compile(const unsigned char buffer[], std::size_t bufferLength) override;
        ~ShaderResource() override;
        [[nodiscard]] unsigned int getType() const;
        static void addPreprocessorSymbol(const std::string& name, const std::string& value);
        static void setPreprocessorPrefix(const std::string& prefix);
        static void setPreprocessorSuffix(const std::string& suffix);
    protected:
        using StringResource::getString;
    private:
        unsigned int type;
        static std::string preprocessorPrefix;
        static std::string preprocessorSuffix;
        static std::unordered_map<std::string, std::string> preprocessorSymbols;
        void checkForCompilationErrors() const;
    };
}
