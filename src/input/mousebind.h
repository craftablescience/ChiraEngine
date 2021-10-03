#pragma once

#include <functional>

namespace chira {
    enum mouseActions {
        MOVE,
        SCROLL
    };

    class mousebind {
    public:
        mousebind(const mouseActions& type, const std::function<void(double,double)>& function);
        [[nodiscard]] mouseActions getType() const;
        void run(double x, double y);
    private:
        mouseActions type;
        std::function<void(double,double)> fire;
    };
}
