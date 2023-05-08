#pragma once

#ifdef CHIRA_USE_RENDER_BACKEND_GL
    #include "device/DeviceGL.h"
#else
    #error "No render device present!"
#endif
