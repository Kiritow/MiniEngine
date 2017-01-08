#pragma once

#ifdef __C4DROID__
#define _WINDOW_PROGRAM
#endif // __C4DROID__

#include <cstdio>
#include <cstdlib>
#include <cstring>

/// If you have configured SDL2 header files,
/// they will be used first.

#include "SDL2/SDL.h"
#undef main

#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

class NonCopyable
{
public:
    NonCopyable()=default;
    ~NonCopyable()=default;
    NonCopyable(const NonCopyable&)=delete;
    NonCopyable& operator = (const NonCopyable&)=delete;
};

extern FILE* _log_fp;

#ifdef _WINDOW_PROGRAM
#define mlog_init() _log_fp=fopen("mini_engine_log.txt","w")
#define mlog(fmt,args...) do {if(_log_fp) {fprintf(_log_fp,fmt,##args);fprintf(_log_fp,"\n");fflush(_log_fp);} }while(0)
#else
#define mlog_init()
#define mlog(fmt,args...) printf(fmt,##args);printf("\n")
#endif
