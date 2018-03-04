#pragma once

/// Include SDL Library Headers.
#include "SDL2/SDL.h"
#undef main
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

/// Version Requiring Definition
#define _MINIENGINE_SDL_VERSION_ATLEAST(X,Y,Z) SDL_VERSION_ATLEAST(X,Y,Z)

/// Deprecated Definition
#define _DECL_DEPRECATED [[deprecated]]
