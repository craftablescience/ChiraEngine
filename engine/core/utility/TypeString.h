#pragma once

#include <string>
#include <sstream>

#include "../Platform.h"
#include "Concepts.h"

// todo: fix this file so it's not generating types for AngelScript

#if defined(CHIRA_COMPILER_GNU) || defined(CHIRA_COMPILER_CLANG)
    #include <cxxabi.h>
    #define CHIRA_USE_DEMANGLED_TYPES
#endif

/*
 * Code contents adapted from the first half of https://accu.org/journals/overload/18/95/golodetz_1608/
 * Notable changes include using variadic templates to register functions with any amount of arguments,
 * getting the name of a type as a string dynamically, and fixing registration of pointers to primitives.
 */

namespace chira {

struct TypeStringBase {
    std::string name, prefix, suffix;
    explicit TypeStringBase(const std::string& name_) {
        this->name = name_;
    }
    explicit operator std::string() const {
        std::ostringstream os;
        if (!this->prefix.empty()) os << this->prefix << ' ';
        os << type();
        if (!this->suffix.empty()) os << ' ' << this->suffix;
        if (!this->name.empty()) os << ' ' << this->name;
        return os.str();
    }
    [[nodiscard]] virtual std::string type() const = 0;
    virtual TypeStringBase& as_param() {
        return *this;
    }
};

template<typename T>
struct TypeString : TypeStringBase {
    explicit TypeString(const std::string& name_ = "") : TypeStringBase(name_) {}
    [[nodiscard]] std::string type() const override {
        std::string type = typeid(T).name();
#ifdef CHIRA_USE_DEMANGLED_TYPES
        int status;
        char* demangled_name = abi::__cxa_demangle(type.c_str(), nullptr, nullptr, &status);
        if (status == 0) {
            type = demangled_name;
            std::free(demangled_name);
        }
#endif
        return type;
    }
};

template<typename T>
std::string typeString() {
    return std::string{TypeString<T>()};
}

template<typename T>
struct TypeString<const T> : TypeString<T> {
    explicit TypeString(const std::string& name_ = "") : TypeString<T>(name_) {
        this->prefix = "const";
    }
    TypeString& as_param() {
        return *this;
    }
};

template<CArithmetic T>
struct TypeString<T*> : TypeString<T> {
    explicit TypeString(const std::string& name_ = "") : TypeString<T>(name_) {
        this->suffix = "&";
    }
    TypeString& as_param() {
        this->suffix = "&out";
        return *this;
    }
};

template<CNonArithmetic T>
struct TypeString<T*> : TypeString<T> {
    explicit TypeString(const std::string& name_ = "") : TypeString<T>(name_) {
        this->suffix = "@";
    }
};

template<typename T>
struct TypeString<T&> : TypeString<T> {
    explicit TypeString(const std::string& name_ = "") : TypeString<T>(name_) {
        this->suffix = "&";
    }
    TypeString& as_param() {
        this->suffix = "&out";
        return *this;
    }
};

template<typename T>
struct TypeString<const T&> : TypeString<T> {
    explicit TypeString(const std::string& name_ = "") : TypeString<T>(name_) {
        this->prefix = "const";
        this->suffix = "&";
    }
    TypeString& as_param() {
        this->suffix = "&in";
        return *this;
    }
};

// Specialize string type
template<>
struct TypeString<std::string> : TypeStringBase {
    explicit TypeString(const std::string& name_ = "") : TypeStringBase(name_) {}
    [[nodiscard]] std::string type() const override {
        return "string";
    }
};

template<typename R, typename... ArgTypes>
struct TypeString<R(ArgTypes...)> {
    std::string name;
    explicit TypeString(const std::string& name_) {
        this->name = name_;
    }
    std::string operator()() const {
        std::ostringstream os;
        os << TypeString<R>()() << ' ' << this->name << '(';
        if constexpr (sizeof...(ArgTypes) > 0) {
            ((os << TypeString<ArgTypes>().as_param()() << ","), ...);
            os.seekp(-1, std::stringstream::cur);
        }
        os << ")";
        return os.str();
    }
};

// Cool template black magic
template<typename R, typename... ArgTypes>
struct TypeString<R(*)(ArgTypes...)> : TypeString<R(ArgTypes...)> {
    explicit TypeString(const std::string& name_) : TypeString<R(ArgTypes...)>(name_) {}
};

} // namespace chira
