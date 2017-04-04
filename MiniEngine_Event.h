#pragma once
#include "MiniEngine.h"

class EventBase
{
protected:
    SDL_Event e;
};

class EventHandler
{
public:
    virtual bool event(const EventBase&)=0;
};
