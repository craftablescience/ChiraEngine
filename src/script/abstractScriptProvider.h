#pragma once

namespace chira {
    class abstractScriptProvider {
    public:
        virtual void initProvider() = 0;
        virtual void initScripts() = 0;
        virtual void render(double delta) = 0;
        virtual void stop() = 0;
    };
}
