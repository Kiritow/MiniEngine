#pragma once
#include "MiniEngine.h"
#include <map>
#include <list>

typedef SDL_Event Event;

int PollEvent(Event& refEvent);
int WaitEvent(Event& refEvent);
int WaitEventTimeout(Event& refEvent,int ms);
int PushEvent(const Event& refEvent);

class Looper
{
public:
    Looper();
    /// If Callback does not return 0, then stop transferring this event.
    void add(decltype(Event::type) event_type,const std::function<int(Looper&,Event&)>& event_callback);
    /// If Callback does not return 0, then stop transferring this event. (Discards Looper)
    void add(decltype(Event::type) event_type,const std::function<int(Event&)>& event_callback);
    /// If Callback has no return (void), then we assume it return 0.
    void add(decltype(Event::type) event_type,const std::function<void(Looper&,Event&)>& event_callback);
    /// If Callback has no return (void), then we assume it return 0. (Discards Looper)
    void add(decltype(Event::type) event_type,const std::function<void(Event&)>& event_callback);
    void dispatch();
    void run();
    Event GetLastEvent();
    void needupdate();
    void needstop();
    void stop();
    void reset();
private:
    Event _e;
    bool _running,_update;
    std::map<decltype(Event::type),std::list<std::function<int(Looper&,Event&)>>> _evmap;
};
