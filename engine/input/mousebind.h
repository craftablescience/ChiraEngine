#pragma once

#include <functional>

namespace chira {
    enum class MouseActions {
        MOVE,
        SCROLL
    };

    class Mousebind {
    public:
        Mousebind(MouseActions type, const std::function<void(double,double)>& function);
        [[nodiscard]] MouseActions getType() const;
        void run(double x, double y);
    private:
        MouseActions type;
        std::function<void(double,double)> fire;
    };
}
