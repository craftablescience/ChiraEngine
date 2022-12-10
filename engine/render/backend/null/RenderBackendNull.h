#pragma once

#include <string_view>
#include <math/Types.h>
#include "../RenderTypes.h"

namespace chira {

struct RenderBackendNull {
    RenderBackendNull() = delete;

    [[nodiscard]] static inline std::string_view getHumanName() { return "Null"; }
    [[nodiscard]] static inline bool setupForDebugging() { return true; }

    // todo(render): stub all other funcs when backend is more stable
};

} // namespace chira
