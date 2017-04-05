#include "MiniEngine_Event.h"

void EventBase::update(const EventBase& ev)
{
    e=ev.e;
}

void MouseMotionEvent::update(const EventBase& ev)
{
    EventBase::update(ev);
    x=e.motion.x;
    y=e.motion.y;
}

void MouseButtonEvent::update(const EventBase& ev)
{
    EventBase::update(ev);
    x=e.button.x;
    y=e.button.y;
}

bool EventHandlerBase::event(const EventBase& ev)
{
    switch(ev.e.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        {
            MouseButtonEvent eb;
            eb.update(ev);
            return onMouseDown(eb);
        }
        break;
    case SDL_MOUSEBUTTONUP:
        {
            MouseButtonEvent eb;
            eb.update(ev);
            return onMouseUp(eb);
        }
        break;
    }

    return false;
}

bool EventHandlerBase::onMouseDown()
{
    return false;
}

bool EventHandlerBase::onMouseUp()
{
    return false;
}
