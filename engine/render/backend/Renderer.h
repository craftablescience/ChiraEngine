#pragma once

#if defined(CHIRA_USE_GL_41) || defined(CHIRA_USE_GL_43)
    #include "RenderBackendGL.h"
#else
    #include "RenderBackendNull.h"
#endif
