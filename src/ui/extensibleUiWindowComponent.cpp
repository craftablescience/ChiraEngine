#include "extensibleUiWindowComponent.h"

using namespace chira;

extensibleUiWindowComponent::extensibleUiWindowComponent(const std::string& title, bool startVisible, const std::function<void(double)>& drawFunc, const ImVec2& windowSize, bool enforceSize) : abstractUiWindowComponent(title, startVisible, windowSize, enforceSize) {
    this->drawFunction = drawFunc;
}

void extensibleUiWindowComponent::draw(double delta) {
    this->drawFunction(delta);
}
