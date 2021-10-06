#pragma once

#include "angelscriptProvider.h"

namespace chira {
    class angelscriptProvider;

    class angelscriptHolder {
    public:
        explicit angelscriptHolder(const std::string& path);
        virtual ~angelscriptHolder();
        void init(angelscriptProvider* provider);
        void render(angelscriptProvider* provider, double delta);
        void stop(angelscriptProvider* provider);
        std::string getIdentifier() {
            return this->identifier;
        }
    private:
        std::string identifier;
        asIScriptContext* scriptContext = nullptr;
        asIScriptFunction* initFunc = nullptr;
        asIScriptFunction* renderFunc = nullptr;
        asIScriptFunction* stopFunc = nullptr;
    };
}
