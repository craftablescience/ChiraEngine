#pragma once

#if defined(CHIRA_USE_RENDER_BACKEND_GL)
    #include "device/DeviceGL.h"
#elif defined(CHIRA_USE_RENDER_BACKEND_SDLRENDERER)
    #include "device/DeviceSDL.h"
#else
    #error "No render device present!"
#endif
