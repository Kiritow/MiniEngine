#pragma once
#include "begin_code.h"
enum class SDLInitFlag
{
    Timer,
    Audio,
    Video, /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
    Joystick, /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
    Haptic,
    GameController, /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
    Events,
    All
};
#include "end_code.h"
