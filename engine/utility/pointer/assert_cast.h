#pragma once

#include "../assertions.h"

#ifdef DEBUG
template<typename T, typename U>
T assert_cast(U obj) {
    auto cast = dynamic_cast<T>(obj);
    // todo(i18n)
    chira_assert(cast, "Object could not be cast");
    return cast;
}
#else
#define assert_cast static_cast
#endif
