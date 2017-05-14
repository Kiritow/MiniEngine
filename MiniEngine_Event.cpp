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

void PumpEvents()
{
    SDL_PumpEvents();
}

bool HasEvent(_SDLEventType_ EventType)
{
    return ( SDL_HasEvent(EventType)==SDL_TRUE );
}

bool HasEvent(_SDLEventType_ EventTypeMin,_SDLEventType_ EventTypeMax)
{
    return ( SDL_HasEvents(EventTypeMin,EventTypeMax)==SDL_TRUE );
}

bool operator == (const LooperID& a,const LooperID& b)
{
    return a._type_id==b._type_id && a._looper_cnt==b._looper_cnt ;
}

bool operator != (const LooperID& a,const LooperID& b)
{
    return !(a==b);
}

Looper::Looper()
{
    _update=_running=true;
    _loop_cnt=0;
    updater=[](){};
}
LooperID Looper::add(_SDLEventType_ event_type,const std::function<int(Looper&,Event&)>& event_callback)
{
    _evmap[event_type].push_front(std::make_pair(_loop_cnt,event_callback));
	return _getNextID(event_type);
}
LooperID Looper::add(_SDLEventType_ event_type,const std::function<int(Event&)>& event_callback)
{
    _evmap[event_type].push_front(std::make_pair(_loop_cnt,[=](Looper& lp,Event& ev)->int{return event_callback(ev);}));
	return _getNextID(event_type);
}
LooperID Looper::add(_SDLEventType_ event_type,const std::function<int(Looper&)>& event_callback)
{
    _evmap[event_type].push_front(std::make_pair(_loop_cnt,[=](Looper& lp,Event& ev)->int{return event_callback(lp);}));
	return _getNextID(event_type);
}
LooperID Looper::add(_SDLEventType_ event_type,const std::function<int()>& event_callback)
{
    _evmap[event_type].push_front(std::make_pair(_loop_cnt,[=](Looper& lp,Event& ev)->int{return event_callback();}));
	return _getNextID(event_type);
}

LooperID Looper::add(_SDLEventType_ event_type,const std::function<void(Looper&,Event&)>& event_callback)
{
    return add(event_type,std::function<int(Looper&,Event&)>([=](Looper& lp,Event& ev)->int{event_callback(lp,ev); return 0;}));
}
LooperID Looper::add(_SDLEventType_ event_type,const std::function<void(Event&)>& event_callback)
{
    return add(event_type,std::function<int(Looper&,Event&)>([=](Looper& lp,Event& ev)->int{event_callback(ev); return 0;}));
}
LooperID Looper::add(_SDLEventType_ event_type,const std::function<void(Looper&)>& event_callback)
{
    return add(event_type,std::function<int(Looper&,Event&)>([=](Looper& lp,Event& ev)->int{event_callback(lp); return 0;}));
}
LooperID Looper::add(_SDLEventType_ event_type,const std::function<void()>& event_callback)
{
    return add(event_type,std::function<int(Looper&,Event&)>([=](Looper& lp,Event& ev)->int{event_callback();return 0;}));
}

LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<int(Looper&,Event&)>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}
LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<int(Event&)>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}
LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<int(Looper&)>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}
LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<int()>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}

LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<void(Looper&,Event&)>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}
LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<void(Event&)>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}
LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<void(Looper&)>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}
LooperID Looper::operator + (const std::pair<_SDLEventType_,std::function<void()>>& event_callback)
{
    return add(event_callback.first,event_callback.second);
}


bool Looper::remove(const LooperID& looperid)
{
    for(auto beginIter=_evmap[looperid._type_id].begin(),
        endIter=_evmap[looperid._type_id].end(),
        iter=beginIter;
    iter!=endIter;
    ++iter)
    {
        if(iter->first==looperid._looper_cnt)
        {
            _evmap[looperid._type_id].erase(iter);
            return true;
        }
    }

    return false;
}

bool Looper::operator -(const LooperID& looperid)
{
    return remove(looperid);
}

void Looper::dispatch()
{
    for(auto callbackPack:_evmap[_e.type])
    {
        if(callbackPack.second(*this,_e)) break;
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

        updater();

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
    _loop_cnt=0;
    updater=[](){};
}

LooperID Looper::_getNextID(const _SDLEventType_& event_type)
{
	LooperID id;
	id._looper_cnt = _loop_cnt;
	id._type_id = event_type;

	++_loop_cnt;
	return id;
}

Poller::Poller()
{
    idler=[](){};
}

void Poller::reset()
{
    Looper::reset();
    idler=[](){};
}

void Poller::run()
{
    int pollret=1;
    while(_running)
    {
        while(!_update&&(pollret=PollEvent(_e)))
        {
            dispatch();
        }

        /// If pollret is not 0 (new event requests update), or pollret is 0 (No New Event) but Idle function requests update, then call updater.
        if(!pollret) idler();
        if(_update)
        {
            updater();
            _update=false;
        }
    }
}

LooperWithTime::LooperWithTime(int Timeout_ms)
{
	_timeout_ms = Timeout_ms;
}

void LooperWithTime::setTimeout(int ms)
{
	_timeout_ms = ms;
}

int LooperWithTime::getTimeout() const
{
	return _timeout_ms;
}

void LooperWithTime::run()
{
	int timeret = 1;
	while (_running)
	{
		while (!_update&&(timeret=WaitEventTimeout(_e, _timeout_ms)))
		{
			dispatch();
		}

		/// If timeret is not 0 (new event request update), or timeret is 0 (Time out) but Idle function requests update, then call updater.
		if (!timeret) idler();
		if (_update)
		{
			updater();
			_update = false;
		}
	}
}
