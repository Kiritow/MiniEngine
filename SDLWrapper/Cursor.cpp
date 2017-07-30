#include "Cursor.h"
#include "_caster.h"
namespace MiniEngine
{
//private
void Cursor::_set(SDL_Cursor* p)
{
    _cur.reset(p,SDL_FreeCursor);
}

//private
void Cursor::_set_no_delete(SDL_Cursor* p)
{
    _cur.reset(p,[](SDL_Cursor* p) {});
}

//private
SDL_Cursor* Cursor::_get()
{
    return _cur.get();
}

//private
void Cursor::_clear()
{
    _cur.release();
}

Cursor::Cursor(Surface surf,Point hotspot)
{
    _set(SDL_CreateColorCursor(surf._get(),hotspot.x,hotspot.y));
}

Cursor::Cursor(SystemCursorType type)
{
    _set(SDL_CreateSystemCursor(_internal::getSDLSystemCursorFromSystemCursorType(type)));
}

//static
Cursor&& Cursor::GetActiveCursor()
{
    Cursor ns;
    ns._set_no_delete(SDL_GetCursor());
    return std::move(ns);
}

//static
Cursor&& Cursor::GetDefaultCursor()
{
    Cursor ns;
    ns._set_no_delete(SDL_GetDefaultCursor());
    return std::move(ns);
}

//static
bool Cursor::isShow()
{
    return (SDL_ShowCursor(SDL_QUERY)==SDL_ENABLE);
}

//static
void Cursor::setShow(bool Settings)
{
    SDL_ShowCursor(Settings?SDL_ENABLE:SDL_DISABLE);
}

void Cursor::release()
{
    _clear();
}

void Cursor::activate()
{
    if(_get()!=nullptr)
    {
        SDL_SetCursor(_get());
    }
}
} /// End of namespace MiniEngine 

