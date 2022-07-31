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
    constexpr Property(T Class::*member_, std::string_view name_, T(Class::*memberFuncGetter_)(), void(Class::*memberFuncSetter_)(T))
            : member(member_)
            , name(name_)
            , memberFuncGetter(memberFuncGetter_)
            , memberFuncSetter(memberFuncSetter_) {}

    using Type = T;

    T Class::*member;
    std::string_view name;
    T(Class::*memberFuncGetter)();
    void(Class::*memberFuncSetter)(T);
};

// https://stackoverflow.com/questions/20405569/in-function-call-why-doesnt-nullptr-match-a-pointer-to-a-template-object
// what the fuck C++
template<class Class, typename T>
constexpr auto createProperty(T Class::*member, std::string_view name, T(Class::*memberFuncGetter)() = static_cast<T(Class::*)()>(nullptr), void(Class::*memberFuncSetter)(T) = static_cast<void(Class::*)(T)>(nullptr)) {
    return Property<Class, T>{member, name, memberFuncGetter, memberFuncSetter};
}

template<class C>
void fromJSON(C* obj, const nlohmann::json& data) {
    const auto numProperties = std::tuple_size<decltype(C::props)>::value;
    forSequence(std::make_index_sequence<numProperties>{}, [&](auto i) {
        const auto property = std::get<i>(C::props);
        using PropType = typename decltype(property)::Type;
        if (data.contains(property.name)) {
            const auto value = data.at(property.name).template get<PropType>();
            if (property.memberFuncSetter) {
                (obj->*(property.memberFuncSetter))(value);
            } else {
                obj->*(property.member) = value;
            }
        }
    });
}

template<class C>
nlohmann::json toJSON(C& obj) {
    nlohmann::json out;

    const auto numProperties = std::tuple_size<decltype(C::props)>::value;
    forSequence(std::make_index_sequence<numProperties>{}, [&](auto i) {
        const auto property = std::get<i>(C::props);
        if (property.memberFuncGetter) {
            out[property.name] = (obj.*(property.memberFuncGetter))();
        } else {
            out[property.name] = obj.*(property.member);
        }
    });

    return out;
}

} // namespace chira::Serialize

#define CHIRA_PROPS static inline const auto props = std::make_tuple
#define CHIRA_PROP(clazz, member) chira::Serialize::createProperty(&clazz::member, #member)
#define CHIRA_PROP_NAMED(clazz, member, name) chira::Serialize::createProperty(&clazz::member, #name)
#define CHIRA_PROP_GET(clazz, member, getter) chira::Serialize::createProperty(&clazz::member, #member, &clazz::getter)
#define CHIRA_PROP_NAMED_GET(clazz, member, name, getter) chira::Serialize::createProperty(&clazz::member, #name, &clazz::getter)
#define CHIRA_PROP_SET(clazz, member, setter) chira::Serialize::createProperty(&clazz::member, #member, static_cast<decltype(member)(clazz::*)()>(nullptr), &clazz::setter)
#define CHIRA_PROP_NAMED_SET(clazz, member, name, setter) chira::Serialize::createProperty(&clazz::member, #name, static_cast<decltype(member)(clazz::*)()>(nullptr), &clazz::setter)
#define CHIRA_PROP_GET_SET(clazz, member, getter, setter) chira::Serialize::createProperty(&clazz::member, #member, &clazz::getter, &clazz::setter)
#define CHIRA_PROP_NAMED_GET_SET(clazz, member, name, getter, setter) chira::Serialize::createProperty(&clazz::member, #name, &clazz::getter, &clazz::setter)
