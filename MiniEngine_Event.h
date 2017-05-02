#pragma once
#include "MiniEngine.h"

typedef SDL_Event Event;

int PollEvent(Event& refEvent);
int WaitEvent(Event& refEvent);
int WaitEventTimeout(Event& refEvent,int ms);
int PushEvent(const Event& refEvent);
