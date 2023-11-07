#include "Assertions.h"

#include <string>

#include "Logger.h"

#ifdef DEBUG
    #include <render/backend/RenderDevice.h>
#endif

using namespace chira;

CHIRA_CREATE_LOG(ASSERT);

#ifdef CHIRA_USE_SOURCE_LOCATION
void chiraAssert(bool shouldAssert, std::string_view message, const std::source_location& location) {
    chiraAssertInternal(shouldAssert, message, location.file_name(), location.line(), location.function_name());
}
#endif

void chiraAssertInternal(bool shouldAssert, std::string_view message, const char* file, unsigned int line, const char* function) {
    // Assertions fail when false
    if (shouldAssert)
        return;

    auto assertMsg = std::string{"Assertion failed at "} + file + "::" + function + "::L" + std::to_string(line) + ":\n" + message.data();
    LOG_ASSERT.error(assertMsg);

#ifdef DEBUG
    if (!chira::Device::popupChoice(assertMsg + "\n\nPress Break to break in debugger, Continue to continue.", "Assertion Failed", chira::Device::POPUP_ERROR, "Continue", "Break")) {
        chira::breakInDebugger();
    }
#endif
}
