#pragma once
#include "include.h"
#include <functional>
#include "begin_code.h"
Uint32 _global_timer_executor(Uint32 interval,void* param);

class Timer
{
public:
    Timer();

    /// void func(Uint32,...)
    template<typename VoidCallable,typename... Args>
    Timer(Uint32 interval,VoidCallable&& vcallable,Args&&... args) : Timer()
    {
        auto realCall=[&,vcallable](Uint32 ims)->Uint32{vcallable(ims,args...); return ims;};
        auto pfunc=new std::function<Uint32(Uint32 interval)>(realCall);
        _real_timer_call(_global_timer_executor,interval,pfunc);
    }

    /// Uint32 func(Uint32,...)
    template<typename Callable,typename... Args>
    Timer(Callable&& callable,Uint32 interval,Args&&... args) : Timer()
    {
        auto realCall=[&,callable](Uint32 ims)->Uint32{return callable(ims,args...);};
        auto pfunc=new std::function<Uint32(Uint32 interval)>(realCall);
        _real_timer_call(_global_timer_executor,interval,pfunc);
    }

    /// Restore For Capability
    Timer(SDL_TimerCallback callback,Uint32 interval,void* param);

    int enable();
    int disable();
    bool isenable() const;
    void detach();
    ~Timer();

    static void _delete_delegator(std::function<Uint32(Uint32)>* Delegator);
private:
    void _real_timer_call(SDL_TimerCallback callback,Uint32 interval,void* param);
    SDL_TimerCallback _callback;
    Uint32 _interval;
    void* _param;
    SDL_TimerID id;
    bool _enabled;
    bool _detached;
    /// Reserved Variable For Template variable Parameter
    bool _delete_on_disable;
};
#include "end_code.h"
