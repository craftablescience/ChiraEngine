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
        std::string getFilePath() {
            return this->filepath;
        }
    private:
        std::string filepath;
        asIScriptContext* scriptContext = nullptr;
        asIScriptFunction* initFunc = nullptr;
        asIScriptFunction* renderFunc = nullptr;
        asIScriptFunction* stopFunc = nullptr;
    };
}
