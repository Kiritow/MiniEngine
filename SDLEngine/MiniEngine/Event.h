#pragma once
#include "../config.h"
#include <functional>

//#include <SDL2/SDL_events.h>
namespace MiniEngine
{

class MouseEvent
{
private:
    struct EventData
    {
        int x,y;
    }
public:
    int type;
    EventData data;
};
class MouseEventListener
{
public:
    MouseEventListener();

    function<void(const MouseEvent&)> onButtonDown;
    function<void(const MouseEvent&)> onButtonUp;

    function<void(const MouseEvent&)> onMotion;
    function<void(const MouseEvent&)> onWheel;
};

}/// End of namespace MiniEngine
