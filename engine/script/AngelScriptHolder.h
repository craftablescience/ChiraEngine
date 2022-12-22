#pragma once

#include <string_view>
#include <functional>
#include <tuple>
#include <type_traits>
#include <core/Logger.h>
#include "AngelScriptVM.h"

namespace chira {

// CHIRA_GET_LOG(ANGELSCRIPT);

class AngelScriptHolder {
public:
    explicit AngelScriptHolder(std::string identifier_);
    ~AngelScriptHolder();
    [[nodiscard]] std::string_view getIdentifier() const {
        return this->identifier;
    }

    template<typename R, typename... Args>
    [[nodiscard]] R callFunction(std::string_view funcName, Args... args) const {

        const auto funcNameFull = asTypeString<R(Args...)>(funcName.data())();

        if (auto* func = this->module->GetFunctionByDecl(funcNameFull.c_str()); func) {
            this->context->Prepare(func);

            const auto addArg = [&] <typename T> (int& argNum, T arg) {
                // todo(as): handle non-primitives
                // todo(as): check if its a pointer or a reference
                if constexpr (std::is_arithmetic_v<T>) {
                    if constexpr (std::is_same_v<float, T>) {
                        this->context->SetArgFloat(argNum, arg);
                    } else if constexpr (std::is_same_v<double, T>) {
                        this->context->SetArgDouble(argNum, arg);
                    } else if constexpr (sizeof(T) == 1) {
                        this->context->SetArgByte(argNum, arg);
                    } else if constexpr (sizeof(T) == 2) {
                        this->context->SetArgWord(argNum, arg);
                    } else if constexpr (sizeof(T) == 4) {
                        this->context->SetArgDWord(argNum, arg);
                    } else if constexpr (sizeof(T) == 8) {
                        this->context->SetArgQWord(argNum, arg);
                    }
                } else {
                    // todo(as): handle non-primitives
                }
                argNum++;
            };

            int argNum = 0;
            (addArg.template operator()<decltype(args)>(argNum, args), ...);

            if (int r = this->context->Execute(); r != asEXECUTION_FINISHED) {
                // LOG_ANGELSCRIPT.error("An exception in \"{}\" occurred:\n{}", this->identifier, this->context->GetExceptionString());
            }

            if constexpr (std::is_same_v<R, void>) {
                return;
            } else {
                // todo(as): handle non-primitives
                // todo(as): check if its a pointer or a reference
                if constexpr (std::is_arithmetic_v<R>) {
                    if constexpr (std::is_same_v<float, R>) {
                        return this->context->GetReturnFloat();
                    } else if constexpr (std::is_same_v<double, R>) {
                        return this->context->GetReturnDouble();
                    } else if constexpr (sizeof(R) == 1) {
                        return this->context->GetReturnByte();
                    } else if constexpr (sizeof(R) == 2) {
                        return this->context->GetReturnWord();
                    } else if constexpr (sizeof(R) == 4) {
                        return this->context->GetReturnDWord();
                    } else if constexpr (sizeof(R) == 8) {
                        return this->context->GetReturnQWord();
                    } else {
                        // todo(as): show an error message or smth
                        return {};
                    }
                } else {
                    // todo(as): show an error message or smth
                    return {};
                }
            }
        } else {
            // LOG_ANGELSCRIPT.error(R"(Script at "{}" does not have a function with signature "{}")", this->identifier, funcNameFull);
            if constexpr (std::is_same_v<R, void>) {
                return;
            } else {
                return {};
            }
        }
    }

private:
    std::string identifier;
    asIScriptContext* context = nullptr;
    asIScriptModule* module = nullptr;
};

} // namespace chira
