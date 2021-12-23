#include "mousebind.h"

using namespace chira;

mousebind::mousebind(const mouseActions& type, const std::function<void(double,double)>& function) {
    this->type = type;
    this->fire = function;
}

mouseActions mousebind::getType() const {
    return this->type;
}

void mousebind::run(double x, double y) {
    this->fire(x, y);
}
