#pragma once
/*
    Base Script Engine

    Used by scripting languages to allow for a more modular
    implementation of scripting languages.
*/

namespace chira {

class ScriptEngine {
    public:
        static void init();
        static void shutdown();
};

} // namespace chira