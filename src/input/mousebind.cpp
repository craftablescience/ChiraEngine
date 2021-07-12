#include "mousebind.h"

mousebind::mousebind(mouseActions type, const std::function<void(engine*,double,double)>& function) {
    this->type = type;
    this->fire = function;
}

mouseActions mousebind::getType() const {
    return this->type;
}

void mousebind::run(engine* engine, double x, double y) {
    this->fire(engine, x, y);
}
