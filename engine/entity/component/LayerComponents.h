#pragma once

#include <tuple>

namespace chira {

struct Layer0Component { static constexpr int index = 1 << 0; };
struct Layer1Component { static constexpr int index = 1 << 1; };
struct Layer2Component { static constexpr int index = 1 << 2; };
struct Layer3Component { static constexpr int index = 1 << 3; };
struct Layer4Component { static constexpr int index = 1 << 4; };
struct Layer5Component { static constexpr int index = 1 << 5; };
struct Layer6Component { static constexpr int index = 1 << 6; };
struct Layer7Component { static constexpr int index = 1 << 7; };
struct Layer8Component { static constexpr int index = 1 << 8; };
struct Layer9Component { static constexpr int index = 1 << 9; };

static constexpr auto LAYER_COMPONENTS = std::tuple<
        Layer0Component, Layer1Component, Layer2Component, Layer3Component, Layer4Component,
        Layer5Component, Layer6Component, Layer7Component, Layer8Component, Layer9Component>{};

} // namespace chira
