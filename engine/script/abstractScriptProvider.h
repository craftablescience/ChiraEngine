#pragma once

namespace chira {
    class AbstractScriptProvider {
    public:
        virtual void init() = 0;
        virtual void render(double delta) = 0;
        virtual void stop() = 0;
        virtual ~AbstractScriptProvider() = default;
    };
}
