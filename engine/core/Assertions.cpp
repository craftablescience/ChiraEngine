#include "Assertions.h"

#include <string>

#include "Logger.h"

#ifdef DEBUG
    #include <utility/Dialogs.h>
#endif

using namespace chira;

CHIRA_CREATE_LOG(ASSERT);

#ifdef CHIRA_USE_SOURCE_LOCATION
void runtime_assert(bool shouldAssert, std::string_view message, const std::source_location& location) {
    // Assertions fail when false
    if (shouldAssert)
        return;

    auto assertMsg = std::string{"Assertion failed!"} + "\n\n" +
            "In file: " + location.file_name() + '\n' +
            "In function: " + location.function_name() + '\n' +
            "At line: " + std::to_string(location.line()) + "\n\n" +
            message.data();
    LOG_ASSERT.error(assertMsg);

#ifdef DEBUG
    if (!chira::Dialogs::popupErrorChoice(assertMsg + "\n\nPress OK to continue, CANCEL to break in debugger.", false, "Assertion Failed"))
        chira::breakInDebugger();
#endif
}
#else
void runtime_assert_internal(bool shouldAssert, std::string_view message, const char* file, int line, const char* function) {
    // Assertions fail when false
    if (shouldAssert)
        return;

    auto assertMsg = std::string{"Assertion failed!"} + "\n\n" +
                     "In file: " + file + '\n' +
                     "In function: " + function + '\n' +
                     "At line: " + std::to_string(line) + "\n\n" +
                     message.data();
    LOG_ASSERT.error(assertMsg);

#ifdef DEBUG
    if (!chira::Dialogs::popupErrorChoice(assertMsg + "\n\nPress OK to continue, CANCEL to break in debugger.", false, "Assertion Failed"))
        chira::breakInDebugger();
#endif
}
#endif
