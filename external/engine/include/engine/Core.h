#pragma once
#include <signal.h>

#define ENGINE_DEBUG

#ifdef ENGINE_DEBUG
    #define BREAK() raise(SIGTRAP)
#else
    #define BREAK()
#endif

template<class T>
using sptr_t = std::shared_ptr<T>;