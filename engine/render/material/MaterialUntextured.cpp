#include "MaterialUntextured.h"

using namespace chira;

void MaterialUntextured::compile(const nlohmann::json& properties) {
    Reflect::fromJSON(this, properties);
}
