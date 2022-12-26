#pragma once

#if defined(CHIRA_USE_GL_41) || defined(CHIRA_USE_GL_43)
    #include "api/BackendGL.h"
#elif defined(CHIRA_USE_DX11)
    #include "api/BackendDX.h"
#else
    #error "No render backend present!"
#endif
