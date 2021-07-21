#ifndef BASICGAMEENGINE_ANGELSCRIPTPROVIDER_H
#define BASICGAMEENGINE_ANGELSCRIPTPROVIDER_H


#include <angelscript.h>
#include <string>
#include <vector>
#include "../../core/engine.h"
#include "angelscriptHolder.h"

class angelscriptHolder;

class angelscriptProvider {
public:
    explicit angelscriptProvider(engine* gameEngine);
    void init();
    void render(double delta);
    void stop();
    void addScript(angelscriptHolder* script);

    static void print(const std::string& message);
    static void messageCallback(const asSMessageInfo* msg, void* param);

    static inline engine* gameEngine = nullptr;
    asIScriptEngine* asEngine = nullptr;
private:
    bool started;
    std::vector<std::unique_ptr<angelscriptHolder>> scripts;
};


#endif //BASICGAMEENGINE_ANGELSCRIPTPROVIDER_H
