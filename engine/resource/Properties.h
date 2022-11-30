#pragma once

#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <nlohmann/json.hpp>

#include <utility/Types.h>

namespace chira::Serialize {

template<class C, typename T>
struct Property {
    constexpr Property(T C::*member_, std::string_view name_, T(C::*memberFuncGetter_)(), void(C::*memberFuncSetter_)(T))
        : member(member_)
        , name(name_)
        , memberFuncGetter(memberFuncGetter_)
        , memberFuncSetter(memberFuncSetter_) {}

    using Type = T;

    T C::*member;
    std::string_view name;
    T(C::*memberFuncGetter)();
    void(C::*memberFuncSetter)(T);
};

// https://stackoverflow.com/questions/20405569/in-function-call-why-doesnt-nullptr-match-a-pointer-to-a-template-object
// what the fuck C++
template<class C, typename T>
constexpr auto createProperty(T C::*member, std::string_view name, T(C::*memberFuncGetter)() = static_cast<T(C::*)()>(nullptr), void(C::*memberFuncSetter)(T) = static_cast<void(C::*)(T)>(nullptr)) {
    return Property<C, T>{member, name, memberFuncGetter, memberFuncSetter};
}

template<class C>
void fromJSON(C* obj, const nlohmann::json& data) {
    const auto numProperties = std::tuple_size_v<decltype(C::props)>;
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

    // Handle single inheritance
    if constexpr (!std::is_same_v<void, typename C::InheritPropsFromClass>) {
        fromJSON<typename C::InheritPropsFromClass>(obj, data);
    }
}

template<class C>
C fromJSON(const nlohmann::json& data) {
    C obj{};
    fromJSON(&obj, data);
    return obj;
}

/// Helper function for recursion
template<class C>
void toJSON(C& obj, nlohmann::json& out) {
    const auto numProperties = std::tuple_size<decltype(C::props)>::value;
    forSequence(std::make_index_sequence<numProperties>{}, [&](auto i) {
        const auto property = std::get<i>(C::props);
        if (property.memberFuncGetter) {
            out[property.name] = (obj.*(property.memberFuncGetter))();
        } else {
            out[property.name] = obj.*(property.member);
        }
    });

    // Handle single inheritance
    if constexpr (!std::is_same_v<void, typename C::InheritPropsFromClass>) {
        toJSON<typename C::InheritPropsFromClass>(obj, out);
    }
}

nlohmann::json toJSON(auto& obj) {
    nlohmann::json out;
    toJSON(obj, out);
    return out;
}

} // namespace chira::Serialize

#define CHIRA_PROPS_INHERITED(clazz) \
    using InheritPropsFromClass = clazz; \
    static inline const auto props = std::make_tuple
#define CHIRA_PROPS() CHIRA_PROPS_INHERITED(void)

#define CHIRA_PROP(clazz, member) chira::Serialize::createProperty(&clazz::member, #member)
#define CHIRA_PROP_NAMED(clazz, member, name) chira::Serialize::createProperty(&clazz::member, #name)
#define CHIRA_PROP_GET(clazz, member, getter) chira::Serialize::createProperty(&clazz::member, #member, &clazz::getter)
#define CHIRA_PROP_NAMED_GET(clazz, member, name, getter) chira::Serialize::createProperty(&clazz::member, #name, &clazz::getter)
#define CHIRA_PROP_SET(clazz, member, setter) chira::Serialize::createProperty(&clazz::member, #member, static_cast<decltype(member)(clazz::*)()>(nullptr), &clazz::setter)
#define CHIRA_PROP_NAMED_SET(clazz, member, name, setter) chira::Serialize::createProperty(&clazz::member, #name, static_cast<decltype(member)(clazz::*)()>(nullptr), &clazz::setter)
#define CHIRA_PROP_GET_SET(clazz, member, getter, setter) chira::Serialize::createProperty(&clazz::member, #member, &clazz::getter, &clazz::setter)
#define CHIRA_PROP_NAMED_GET_SET(clazz, member, name, getter, setter) chira::Serialize::createProperty(&clazz::member, #name, &clazz::getter, &clazz::setter)
