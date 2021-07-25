#pragma once

#include <functional>
#include "../core/engine.h"

class engine;

enum mouseActions {
    MOVE,
    SCROLL
};

class mousebind {
public:
    explicit mousebind(mouseActions type, const std::function<void(engine*,double,double)>& function);
    [[nodiscard]] mouseActions getType() const;
    void run(engine* engine, double x, double y);
private:
    mouseActions type;
    std::function<void(engine*,double,double)> fire;
};
