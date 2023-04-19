#pragma once

#include <string>
#include <string_view>
#include <tuple>
#include <nlohmann/json.hpp>
#include <math/Types.h>
#include <utility/Types.h>

namespace chira::Reflect {

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
    // Handle single inheritance, deserialize base class first
    if constexpr (!std::is_same_v<void, typename C::InheritPropsFromClass>) {
        fromJSON<typename C::InheritPropsFromClass>(obj, data);
    }

    constexpr auto numProperties = std::tuple_size_v<decltype(C::chira_props)>;
    forSequence(std::make_index_sequence<numProperties>{}, [&](auto i) {
        const auto property = std::get<i>(C::chira_props);
        using PropType = typename decltype(property)::Type;
        if (data.contains(property.name)) {
            if constexpr (requires {PropType::chira_props;}) {
                const auto value = fromJSON(data.at(property.name));
                if (property.memberFuncSetter) {
                    (obj->*(property.memberFuncSetter))(value);
                } else {
                    obj->*(property.member) = value;
                }
            } else {
                const auto value = data.at(property.name).template get<PropType>();
                if (property.memberFuncSetter) {
                    (obj->*(property.memberFuncSetter))(value);
                } else {
                    obj->*(property.member) = value;
                }
            }
        }
    });
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
    // Handle single inheritance, serialize base class first
    if constexpr (!std::is_same_v<void, typename C::InheritPropsFromClass>) {
        toJSON<typename C::InheritPropsFromClass>(obj, out);
    }

    constexpr auto numProperties = std::tuple_size_v<decltype(C::chira_props)>;
    forSequence(std::make_index_sequence<numProperties>{}, [&](auto i) {
        const auto property = std::get<i>(C::chira_props);
        const auto value = property.memberFuncGetter
                ? (obj.*(property.memberFuncGetter))()
                : obj.*(property.member);
        if constexpr (requires {decltype(value)::chira_props;}) {
            toJSON(value, out[property.name]);
        } else {
            out[property.name] = value;
        }
    });
}

nlohmann::json toJSON(auto& obj) {
    nlohmann::json out;
    toJSON(obj, out);
    return out;
}

} // namespace chira::Reflect

#define CHIRA_PROPS_INHERITED(clazz) \
    using InheritPropsFromClass = clazz; \
    static inline const auto chira_props = std::make_tuple
#define CHIRA_PROPS CHIRA_PROPS_INHERITED(void)

#define CHIRA_PROP(clazz, member) chira::Reflect::createProperty(&clazz::member, #member)
#define CHIRA_PROP_NAMED(clazz, member, name) chira::Reflect::createProperty(&clazz::member, #name)
#define CHIRA_PROP_GET(clazz, member, getter) chira::Reflect::createProperty(&clazz::member, #member, &clazz::getter)
#define CHIRA_PROP_NAMED_GET(clazz, member, name, getter) chira::Reflect::createProperty(&clazz::member, #name, &clazz::getter)
#define CHIRA_PROP_SET(clazz, member, setter) chira::Reflect::createProperty(&clazz::member, #member, static_cast<decltype(member)(clazz::*)()>(nullptr), &clazz::setter)
#define CHIRA_PROP_NAMED_SET(clazz, member, name, setter) chira::Reflect::createProperty(&clazz::member, #name, static_cast<decltype(member)(clazz::*)()>(nullptr), &clazz::setter)
#define CHIRA_PROP_GET_SET(clazz, member, getter, setter) chira::Reflect::createProperty(&clazz::member, #member, &clazz::getter, &clazz::setter)
#define CHIRA_PROP_NAMED_GET_SET(clazz, member, name, getter, setter) chira::Reflect:createProperty(&clazz::member, #name, &clazz::getter, &clazz::setter)
