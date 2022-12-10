#pragma once

#if defined(CHIRA_USE_GL_41) || defined(CHIRA_USE_GL_43)
    #include "gl/RenderBackendGL.h"
#else
    #include "null/RenderBackendNull.h"
#endif
