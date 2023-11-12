#pragma once

#include <sstream>
#include <type_traits>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/string.hpp>

namespace chira::Serial {

template<typename T>
void loadFromBuffer(T* that, const std::byte buffer[], std::size_t bufferLength) {
    std::istringstream stream{std::string{reinterpret_cast<const char*>(buffer), bufferLength}};
    cereal::JSONInputArchive archive{stream};
    that->serialize(archive);
}

} // namespace chira::Serial
