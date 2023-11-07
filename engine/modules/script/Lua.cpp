#include "Lua.h"

#include <string>

#include <fmt/args.h>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <core/debug/Logger.h>

using namespace chira;

CHIRA_CREATE_LOG(LUA);

// todo(scripting): pull this into a DLL and register it into a list of providers

static sol::state& luaState() {
    static sol::state state;
    return state;
}

static std::string toString(const sol::object& obj) {
    return luaState()["tostring"](obj).get<std::string>();
}

void Lua::init() {
    auto& lua = luaState();

    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::bit32, sol::lib::math, sol::lib::table, sol::lib::utf8);

    lua.set_function("print", [](const sol::object& message) {
        LOG_LUA.output(::toString(message));
    });

    lua.set_function("printf", [](std::string_view fmtStr, sol::variadic_args va) {
        fmt::dynamic_format_arg_store<fmt::format_context> ds;
        for (auto v : va) {
            ds.push_back(::toString(v));
        }
        LOG_LUA.output(fmt::vformat(fmtStr, ds));
    });

    lua.set_function("fmt", [](std::string_view fmtStr, sol::variadic_args va) {
        fmt::dynamic_format_arg_store<fmt::format_context> ds;
        for (auto v : va) {
            ds.push_back(::toString(v));
        }
        return fmt::vformat(fmtStr, ds);
    });
}

void Lua::script(std::string_view code) {
    luaState().script(code, [](lua_State*, sol::protected_function_result result) {
        if (!result.valid()) {
            sol::error err = result;
            LOG_LUA.error("Failed to run given code: {}", err.what());
        }
        return result;
    });
}
