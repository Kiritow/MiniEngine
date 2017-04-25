#include "MiniEngine_Event.h"

namespace MiniEngine
{

namespace EventSystem
{

namespace _MiniEngine_Internal
{
    MouseButton _mousebutton_event_enum_caster(int EventInt)
    {
        switch(EventInt)
        {
        case SDL_BUTTON_LEFT:
            return MouseButton::Left;
        case SDL_BUTTON_MIDDLE:
            return MouseButton::Middle;
        case SDL_BUTTON_RIGHT:
            return MouseButton::Right;
        case SDL_BUTTON_X1:
            return MouseButton::X1;
        case SDL_BUTTON_X2:
            return MouseButton::X2;

        /// If an error occurs, return Left by default.
        default:
            return MouseButton::Left;
        }
    }
}/// End of namespace MiniEngine::EventSystem::_MiniEngine_Internal

EventBase::EventBase(SDL_Event Event)
{
    e=Event;
}

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
    button=_MiniEngine_Internal::_mousebutton_event_enum_caster(e.button.button);
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
    case SDL_MOUSEMOTION:
        {
            MouseMotionEvent eb;
            eb.update(ev);
            return onMouseMotion(eb);
        }
        break;
    }

    return false;
}

bool EventHandlerBase::onMouseDown(const MouseButtonEvent& ev)
{
    return false;
}

bool EventHandlerBase::onMouseUp(const MouseButtonEvent& ev)
{
    return false;
}

bool EventHandlerBase::onMouseMotion(const MouseMotionEvent& ev)
{
    return false;
}

}/// End of namespace MiniEngine::EventSystem

}/// End of namespace MiniEngine
