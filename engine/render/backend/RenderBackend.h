#pragma once

#ifdef CHIRA_USE_RENDER_BACKEND_GL
    #include "api/BackendGL.h"
#else
    #error "No render backend present!"
#endif
