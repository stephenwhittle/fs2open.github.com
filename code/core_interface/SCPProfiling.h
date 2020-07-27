#pragma once

#ifdef FS2_ENABLE_PROFILING
#include "optick.h"
#define FS2_PROF_APP(...) OPTICK_APP(__VA_ARGS__)
#define FS2_PROF_FRAME(...) OPTICK_FRAME(__VA_ARGS__)
#define FS2_PROF_EVENT(...) OPTICK_EVENT(__VA_ARGS__)
#define FS2_PROF_THREAD(...) OPTICK_THREAD(__VA_ARGS__)
#else
#define FS2_PROF_APP(...)
#define FS2_PROF_FRAME(...)
#define FS2_PROF_EVENT(...)
#define FS2_PROF_THREAD(...)
#endif