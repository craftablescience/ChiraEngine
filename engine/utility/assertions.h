#pragma once

#include <string>
#include "logger.h"
#ifdef DEBUG
#include "dialogs.h"
#endif

namespace chira {
    inline void assert_internal(bool shouldAssert, const std::string& message, const char* FILE, int LINE) {
        // Assertions fail when false
        if (shouldAssert)
            return;
        std::string assertMessage = std::string{"Assertion in file "} + FILE + " at line " + std::to_string(LINE) + '\n' + message;
        logger::log(ERR, "Assert", assertMessage);
#ifdef DEBUG
        dialogPopupError(assertMessage, "Assertion Failed");
#endif
    }
}

#define chira_assert(shouldAssert, message) chira::assert_internal(shouldAssert, message, __FILE__, __LINE__)
