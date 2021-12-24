#pragma once

#include <string>
#include "logger.h"
#ifdef DEBUG
#include "dialogs.h"
#endif

#include <cassert>
#undef assert

namespace chira {
    inline void assert(bool shouldAssert, const std::string& message = "") {
        // Assertions fail when false
        if (shouldAssert)
            return;
        std::string assertMessage = std::string{"Assertion in file "} + __FILE__ + \
                                " at line " + std::to_string(__LINE__) + '\n' + message;
        logger::log(ERR, "Assert", assertMessage);
#ifdef DEBUG
        dialogPopupError(assertMessage, "Assertion Failed");
#endif
    }
}
