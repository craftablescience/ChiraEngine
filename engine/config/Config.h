#pragma once

#include <string>
#include <string_view>

namespace chira::Config {

[[nodiscard]] std::string_view getConfigDirectory();
[[nodiscard]] std::string getConfigFile(std::string_view file);

} // namespace chira::Config
