#pragma once

#include <string_view>
#include <functional>
#include "AngelScriptVM.h"

namespace chira {

class AngelScriptHolder {
public:
    explicit AngelScriptHolder(std::string identifier_);
    ~AngelScriptHolder();
    [[nodiscard]] std::string_view getIdentifier() const {
        return this->identifier;
    }
    // todo: templatize this somehow?
    [[nodiscard]] std::function<void()> getFunction(std::string_view funcName) const;
private:
    std::string identifier;
    asIScriptContext* context = nullptr;
    asIScriptModule* module = nullptr;
};

} // namespace chira
