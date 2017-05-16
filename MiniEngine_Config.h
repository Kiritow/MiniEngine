#pragma once

#ifdef _MSC_VER
/// Visual Studio (VC++ Compiler)
#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

/// VC++ does not implied C++ exception. Use this to ignore compile warning on this.
#pragma warning (disable:4290)
#else
/// CodeBlocks (MinGW Compiler)
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#endif /// End of #ifdef _MSC_VER

#define _DECL_DEPRECATED [[deprecated]]
