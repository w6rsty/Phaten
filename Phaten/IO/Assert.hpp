#pragma once

#include <cassert>

#include "Core/Core.hpp"
#include "Logger.hpp"

namespace Pt
{

/// DO NOT wrap functions in the macro 

/// DO NOT add your exlamation mark in assertion 
/// I already added it for you :)
// and I don't like to see so many exclamation marks in the log :]
#ifdef PT_ENABLE_ASSERTION
    #define PT_ASSERT(x) if (!(x)) \
        do { \
            PT_LOG_FATAL("\x1b[31;1mAssertion failed! \x1b[0m"); \
            assert(false); \
        } while (0)

    #define PT_ASSERT_MSG(x, ...) \
        do { \
            if (!(x)) \
            { \
                PT_LOG_FATAL("\x1b[31;1mAssertion failed! \x1b[0m: ", __VA_ARGS__); \
                assert(false); \
            } \
        } while (0)
#else
    #define PT_ASSERT(x)
    #define PT_ASSERT_MSG(x, ...)
#endif


} // namespace Pt
