#pragma once

#include <string_view>

namespace chira::Config {

[[nodiscard]] std::string_view getConfigDirectory();

} // namespace chira::Config
