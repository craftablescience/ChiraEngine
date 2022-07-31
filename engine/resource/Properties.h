#pragma once

#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <nlohmann/json.hpp>

#include <utility/Types.h>

namespace chira::Serialize {

template<class Class, typename T>
struct Property {
    constexpr Property(T Class::*member_, std::string_view name_)
            : member(member_)
            , name(name_) {}

    using Type = T;

    T Class::*member;
    std::string_view name;
};

template<class Class, typename T>
constexpr auto createProperty(T Class::*member, std::string_view name) {
    return Property<Class, T>{member, name};
}

template<class C>
void fromJSON(C* obj, const nlohmann::json& data) {
    const auto numProperties = std::tuple_size<decltype(C::props)>::value;
    forSequence(std::make_index_sequence<numProperties>{}, [&](auto i) {
        const auto property = std::get<i>(C::props);
        using PropType = typename decltype(property)::Type;
        if (data.contains(property.name)) {
            obj->*(property.member) = data.at(property.name).template get<PropType>();
        }
    });
}

template<class C>
nlohmann::json toJSON(const C& obj) {
    nlohmann::json out;

    const auto numProperties = std::tuple_size<decltype(C::props)>::value;
    forSequence(std::make_index_sequence<numProperties>{}, [&](auto i) {
        const auto property = std::get<i>(C::props);
        out[property.name] = obj.*(property.member);
    });

    return out;
}

} // namespace chira::Serialize

#define CHIRA_PROPS static inline const auto props = std::make_tuple
#define CHIRA_PROP(clazz, member) chira::Serialize::createProperty(&clazz::member, #member)
#define CHIRA_PROP_NAMED(clazz, member, name) chira::Serialize::createProperty(&clazz::member, #name)
