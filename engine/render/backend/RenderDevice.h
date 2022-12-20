#pragma once

#ifndef CHIRA_BUILD_HEADLESS
    #include "device/DeviceSDL.h"
#else
    //todo(render): implement a headless mode for testing
    #error "Headless mode is not implemented yet."
#endif
