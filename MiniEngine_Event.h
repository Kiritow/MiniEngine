#pragma once
#include "MiniEngine.h"
#include <map>
#include <list>

typedef SDL_Event Event;

int PollEvent(Event& refEvent);
int WaitEvent(Event& refEvent);
int WaitEventTimeout(Event& refEvent,int ms);
int PushEvent(const Event& refEvent);

typedef struct
{
    decltype(Event::type) _type_id;
    int _looper_cnt;
}LooperID;

bool operator == (const LooperID&,const LooperID&);

class Looper
{
public:
    typedef decltype(Event::type) _SDLEventType_;
    Looper();
    /// If Callback does not return 0, then stop transferring this event.
    LooperID add(_SDLEventType_ event_type,const std::function<int(Looper&,Event&)>& event_callback);
    LooperID add(_SDLEventType_ event_type,const std::function<int(Event&)>& event_callback);
    LooperID add(_SDLEventType_ event_type,const std::function<int(Looper&)>& event_callback);
    LooperID add(_SDLEventType_ event_type,const std::function<int()>& event_callback);

    /// If Callback has no return (void), then we assume it returns 0.
    LooperID add(_SDLEventType_ event_type,const std::function<void(Looper&,Event&)>& event_callback);
    LooperID add(_SDLEventType_ event_type,const std::function<void(Event&)>& event_callback);
    LooperID add(_SDLEventType_ event_type,const std::function<void(Looper&)>& event_callback);
    LooperID add(_SDLEventType_ event_type,const std::function<void()>& event_callback);

    LooperID operator + (const std::pair<_SDLEventType_,std::function<int(Looper&,Event&)>>& event_callback);
    LooperID operator + (const std::pair<_SDLEventType_,std::function<int(Event&)>>& event_callback);
    LooperID operator + (const std::pair<_SDLEventType_,std::function<int(Looper&)>>& event_callback);
    LooperID operator + (const std::pair<_SDLEventType_,std::function<int()>>& event_callback);

    LooperID operator + (const std::pair<_SDLEventType_,std::function<void(Looper&,Event&)>>& event_callback);
    LooperID operator + (const std::pair<_SDLEventType_,std::function<void(Event&)>>& event_callback);
    LooperID operator + (const std::pair<_SDLEventType_,std::function<void(Looper&)>>& event_callback);
    LooperID operator + (const std::pair<_SDLEventType_,std::function<void()>>& event_callback);

    bool remove(const LooperID& looperid);

    bool operator - (const LooperID& looperid);

    void dispatch();
    void run();
    Event GetLastEvent();
    void needupdate();
    void needstop();
    void stop();
    void reset();

    std::function<void()> updater;

protected:
    Event _e;
    bool _running,_update;
    std::map<_SDLEventType_,std::list<std::pair<int,std::function<int(Looper&,Event&)>>>> _evmap;
    int _loop_cnt;
};

class Poller : public Looper
{
public:
    Poller();
    void run();
    void reset();

    std::function<void()> idler;
};
