#include "mousebind.h"

using namespace chira;

Mousebind::Mousebind(MouseActions type, const std::function<void(double,double)>& function) {
    this->type = type;
    this->fire = function;
}

MouseActions Mousebind::getType() const {
    return this->type;
}

void Mousebind::run(double x, double y) {
    this->fire(x, y);
}
