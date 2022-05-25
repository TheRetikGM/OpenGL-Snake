#pragma once
#include <signal.h>

#define ENGINE_DEBUG

#ifdef ENGINE_DEBUG
#ifdef WIN32
    #define BREAK() __debugbreak()
#else
    #define BREAK() raise(SIGTRAP)
#endif
#else
    #define BREAK()
#endif

template<class T>
using sptr_t = std::shared_ptr<T>;