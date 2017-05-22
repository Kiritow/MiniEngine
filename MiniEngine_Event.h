#pragma once
#include "MiniEngine_Config.h"
#include <map>
#include <list>
#include <functional>

typedef SDL_Event Event;
typedef decltype(Event::type) _SDLEventType_;

int PollEvent(Event& refEvent);
int WaitEvent(Event& refEvent);
int WaitEventTimeout(Event& refEvent,int ms);
int PushEvent(const Event& refEvent);
void PumpEvents();
bool HasEvent(_SDLEventType_ EventType);
bool HasEvent(_SDLEventType_ EventTypeMin,_SDLEventType_ EventTypeMax);
bool EnableEvent(_SDLEventType_ EventType);
bool DisableEvent(_SDLEventType_ EventType);
bool IsEventEnabled(_SDLEventType_ EventType);
_SDLEventType_ RegisterEvent(int howMuch);
bool IsValidEventType(_SDLEventType_ EventType);

typedef struct
{
    _SDLEventType_ _type_id;
    int _looper_cnt;
}LooperID;

bool operator == (const LooperID&,const LooperID&);
bool operator != (const LooperID&,const LooperID&);

class Looper
{
public:
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
	LooperID _getNextID(const _SDLEventType_ & event_type);
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

class LooperWithTime : public Poller
{
public:
	LooperWithTime(int Timeout_ms=10);
	void setTimeout(int ms);
	int getTimeout() const;
	void run();
protected:
	int _timeout_ms;
};
