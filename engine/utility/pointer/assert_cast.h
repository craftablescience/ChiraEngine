#pragma once

#include "../assertions.h"

namespace chira {
    template<typename T, typename U>
    inline T assert_cast(U obj) {
#ifdef DEBUG
        auto cast = dynamic_cast<T>(obj);
        // todo(i18n)
        chira_assert(cast, std::string{"Object in file "} + __FILE__ + " at line " + std::to_string(__LINE__) + " could not be cast");
        return cast;
#else
        return static_cast<T>(obj);
#endif
    }
}
