#pragma once
namespace MiniEngine {
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
} /// End of namespace MiniEngine
