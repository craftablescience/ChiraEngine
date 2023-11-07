#pragma once

#include <string_view>

namespace chira::Lua {

void init();
void script(std::string_view code);

} // namespace chira::Lua
