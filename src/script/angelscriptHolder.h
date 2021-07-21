#ifndef BASICGAMEENGINE_ANGELSCRIPTHOLDER_H
#define BASICGAMEENGINE_ANGELSCRIPTHOLDER_H


#include "angelscriptProvider.h"

class angelscriptProvider;

class angelscriptHolder {
public:
    explicit angelscriptHolder(const std::string& path) {
        this->filepath = path;
    }
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


#endif //BASICGAMEENGINE_ANGELSCRIPTHOLDER_H
