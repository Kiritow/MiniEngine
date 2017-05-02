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


Looper::Looper()
{
    _update=_running=true;
}
void Looper::add(decltype(Event::type) event_type,const std::function<int(Looper&,Event&)>& event_callback)
{
    _evmap[event_type].push_back(event_callback);
}
void Looper::add(decltype(Event::type) event_type,const std::function<int(Event&)>& event_callback)
{
    _evmap[event_type].push_back([&](Looper& lp,Event& ev)->int{return event_callback(ev);});
}
void Looper::add(decltype(Event::type) event_type,const std::function<void(Looper&,Event&)>& event_callback)
{
    _evmap[event_type].push_back([&](Looper& lp,Event& ev)->int{event_callback(lp,ev); return 0;});
}
void Looper::add(decltype(Event::type) event_type,const std::function<void(Event&)>& event_callback)
{
    _evmap[event_type].push_back([&](Looper& lp,Event& ev)->int{event_callback(ev); return 0;});
}
void Looper::dispatch()
{
    for(auto callback:_evmap[_e.type])
    {
        if(callback(*this,_e)) break;
    }
}
void Looper::run()
{
    while(_running)
    {
        while(!_update&&WaitEvent(_e))
        {
            dispatch();
        }

        _update=false;
    }
}
Event Looper::GetLastEvent()
{
    return _e;
}
void Looper::needupdate()
{
    _update=true;
}
void Looper::needstop()
{
    _running=false;
}
void Looper::stop()
{
    needstop();
    needupdate();
}
void Looper::reset()
{
    _running=true;
    _update=true;
    _evmap.clear();
}
