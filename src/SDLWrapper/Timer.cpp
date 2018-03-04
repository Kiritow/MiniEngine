#include "SDLWrapper/Timer.h"
namespace MiniEngine {
/// Global Executor For class Timer
Uint32 _global_timer_executor(Uint32 interval,void* param)
{
    auto p=reinterpret_cast<std::function<Uint32(Uint32 interval)>*>(param);
    return (*p)(interval);
}

Timer::Timer()
{
    _enabled=false;
    _detached=false;
    _delete_on_disable=false;
    id=-1;
}

Timer::Timer(SDL_TimerCallback callback,Uint32 interval,void* param) : Timer()
{
    _real_timer_call(callback,interval,param);
}

void Timer::_real_timer_call(SDL_TimerCallback callback,Uint32 interval,void* param)
{
    _callback=callback;
    _interval=interval;
    _param=param;
}

int Timer::enable()
{
    if(_enabled)
    {
        return -1;
    }
    else
    {
        id=SDL_AddTimer(_interval,_callback,_param);
        if(id<0) return -2;
        _enabled=true;
        return 0;
    }
}

int Timer::disable()
{
    if(_enabled)
    {
        SDL_RemoveTimer(id);
        _enabled=false;
        id=-1;
        _callback=nullptr;

        if(_delete_on_disable)
        {
            _delete_delegator(reinterpret_cast<std::function<Uint32(Uint32 interval)>*>(_param));
            _delete_on_disable=false;
        }

        _param=nullptr;
        return 0;
    }
    else
    {
        return -1;
    }
}

void Timer::detach()
{
    _detached=true;
}

Timer::~Timer()
{
    if(!_detached)
    {
        disable();
    }
}

//static
void Timer::_delete_delegator(std::function<Uint32(Uint32)>* param)
{
    delete param;
}
} /// End of namespace MiniEngine
