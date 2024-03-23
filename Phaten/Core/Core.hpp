#pragma once

namespace Pt
{

#define PT_DEBUG

#ifdef PT_DEBUG
    #define PT_ENABLE_LOGGING
    #define PT_ENABLE_ASSERTION

    #define PT_SHADER_DEBUG
    // Output all shader source code
    // #define PT_SHADER_DEBUG_SHOW
#endif

} // namespace Pt