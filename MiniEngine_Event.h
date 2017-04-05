#pragma once
#include "MiniEngine.h"

class EventBase
{
protected:
    SDL_Event e;
    virtual void update(const EventBase&);
    friend class EventHandlerBase;
};

class MouseMotionEvent : public EventBase
{
public:
    int x,y;
    void update(const EventBase&);
};

class MouseButtonEvent : public EventBase
{
public:
    int x,y;
    int button;
    void update(const EventBase&);
};

class EventHandlerBase
{
public:
    virtual bool event(const EventBase&);
protected:
    virtual bool onMouseDown(const MouseButtonEvent&);
    virtual bool onMouseUp(const MouseButtonEvent&);
};
