#pragma once
#include "MiniEngine.h"

namespace MiniEngine
{

namespace EventSystem
{

class EventBase
{
public:
    EventBase()=default;
    EventBase(SDL_Event);
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

enum class MouseButton { Left, Middle, Right, X1, X2 };

class MouseButtonEvent : public EventBase
{
public:
    int x,y;
    MouseButton button;
    void update(const EventBase&);
};

class EventHandlerBase
{
public:
    virtual bool event(const EventBase&);
protected:
    virtual bool onMouseDown(const MouseButtonEvent&);
    virtual bool onMouseUp(const MouseButtonEvent&);
    virtual bool onMouseMotion(const MouseMotionEvent&);
};

}/// End of namespace MiniEngine::EventSystem

}/// End of namespace MiniEngine
