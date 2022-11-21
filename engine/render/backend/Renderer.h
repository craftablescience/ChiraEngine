#pragma once

#if defined(CHIRA_USE_GL_41) || defined(CHIRA_USE_GL_43)
    #include "RenderBackendGL.h"
    namespace chira { using Renderer = RenderBackendGL; }
#else
    #include "RenderBackendNull.h"
    namespace chira { using Renderer = RenderBackendNull; }
#endif
