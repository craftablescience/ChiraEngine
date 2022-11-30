#include "MaterialUntextured.h"

using namespace chira;

void MaterialUntextured::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);
}
