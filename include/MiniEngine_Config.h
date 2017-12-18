#pragma once

#ifdef _MSC_VER
/// Visual Studio (VC++ Compiler)
/// VC++ does not implied C++ exception. Use this to ignore compile warning on this.
#pragma warning (disable:4290)
#define _COMPILER_LABLE 1
#else
/// CodeBlocks (MinGW Compiler)
#define _COMPILER_LABLE 2
#endif /// End of #ifdef _MSC_VER

#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define _DECL_DEPRECATED [[deprecated]]
