#pragma once
#include "include.h"
#include "SystemCursorType.h"
#include "Point.h"
#include "Surface.h"
#include "__Plugin.h"

namespace MiniEngine {

class Cursor
{
public:
    Cursor()=default;
    Cursor(SystemCursorType);
    Cursor(Surface surf,Point hotspot= {0,0});

    static Cursor GetActiveCursor();
    static Cursor GetDefaultCursor();

    static void setShow(bool);
    static bool isShow();

    void activate();

    void release();
private:
    std::shared_ptr<SDL_Cursor> _cur;
    void _set(SDL_Cursor*);
    void _set_no_delete(SDL_Cursor*);
    SDL_Cursor* _get();
    void _clear();

    friend class _internal::Plugin;
};

} /// End of namespace MiniEngine
