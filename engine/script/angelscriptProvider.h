#pragma once

#include <angelscript.h>
#include <string>
#include <vector>
#include <memory>
#include "abstractScriptProvider.h"
#include "angelscriptHolder.h"
#include "angelscriptHelpers.h"

namespace chira {
    class AngelscriptHolder;

    class AngelscriptProvider : public AbstractScriptProvider {
    public:
        AngelscriptProvider();
        [[nodiscard]] asIScriptEngine* getAngelscriptEngine() const;
        void init() override;
        void render(double delta) override;
        void stop() override;
        void addScript(const std::string& script);

        template<typename F>
        int registerGlobalFunction(F f, const std::string& name) {
            return registerGlobalFunction<F>(f, name, asTypeString<F>(name)());
        }
        template<typename F>
        int registerGlobalFunction(F f, const std::string& name, const std::string& decl) {
            return this->asEngine->RegisterGlobalFunction(decl.c_str(), asFUNCTION(f), asCALL_CDECL);
        }
    private:
        bool started = false;
        asIScriptEngine* asEngine = nullptr;
        std::vector<std::unique_ptr<AngelscriptHolder>> scripts{};
    };
}
