#pragma once

#include <tuple>

namespace chira {

// The maximum amount of layers
constexpr int MAX_LAYER_COMPONENTS = 16;

template<int LayerIndex>
struct LayerComponent {
    static constexpr std::size_t index = 1 << LayerIndex;
};

template<int... Indices>
consteval auto createLayerComponentsTuple(std::integer_sequence<int, Indices...>) {
    static_assert(MAX_LAYER_COMPONENTS > 0, "There must be at least one layer component!");
    return std::make_tuple(LayerComponent<Indices>()...);
}

constexpr auto LAYER_COMPONENTS = createLayerComponentsTuple(std::make_integer_sequence<int, MAX_LAYER_COMPONENTS>{});

} // namespace chira
