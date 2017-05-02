#include "MiniEngine_Event.h"

int PollEvent(Event& refEvent)
{
    return SDL_PollEvent(&refEvent);
}

int WaitEvent(Event& refEvent)
{
    return SDL_WaitEvent(&refEvent);
}

int WaitEventTimeout(Event& refEvent,int ms)
{
    return SDL_WaitEventTimeout(&refEvent,ms);
}

int PushEvent(const Event& refEvent)
{
    return SDL_PushEvent(const_cast<Event*>(&refEvent));
}
