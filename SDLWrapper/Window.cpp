#include "Window.h"
#include "begin_code.h"
//private
void Window::_set(SDL_Window* p)
{
    _wnd.reset(p,SDL_DestroyWindow);
}

//private
void Window::_clear()
{
    _wnd.reset();
}

//private
SDL_Window* Window::_get() const
{
    return _wnd.get();
}

Window::Window(std::string Title, int Width, int Height,
               std::initializer_list<WindowType> WindowFlags, int WindowPositionX, int WindowPositionY) throw(ErrorViewer)
{
    /// Calculate Window Flags
    Uint32 windowFlag=0;
    for(auto v:WindowFlags)
    {
        windowFlag|=_internal::getSDLWindowFlagsFromWindowType(v);
    }

    SDL_Window* temp = SDL_CreateWindow(Title.c_str(), WindowPositionX, WindowPositionY, Width, Height, windowFlag);
    if (temp == NULL)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }

    _set(temp);
}

Rect Window::getSize() const
{
    int w, h;
    SDL_GetWindowSize(_get(), &w, &h);
    return Rect(0, 0, w, h);
}

void Window::setSize(const Rect& sizeRect)
{
    setSize(sizeRect.w, sizeRect.h);
}

void Window::setSize(int w, int h)
{
    SDL_SetWindowSize(_get(), w, h);
}

Point Window::getPosition() const
{
    int x, y;
    SDL_GetWindowPosition(_get(), &x, &y);
    return Point(x, y);
}

void Window::setPosition(int x, int y)
{
    SDL_SetWindowPosition(_get(), x, y);
}

void Window::setPosition(const Point& point)
{
    SDL_SetWindowPosition(_get(), point.x, point.y);
}

void Window::setTitle(const std::string& Title)
{
    SDL_SetWindowTitle(_get(), Title.c_str());
}

std::string Window::getTitle() const
{
    return std::string(SDL_GetWindowTitle(_get()));
}

void Window::setGrab(bool isGrab)
{
    SDL_SetWindowGrab(_get(),isGrab?SDL_TRUE:SDL_FALSE);
}

bool Window::getGrab() const
{
    return (SDL_GetWindowGrab(_get())==SDL_TRUE)?true:false;
}

#if _MINIENGINE_SDL_VERSION_ATLEAST(2,0,5)
int Window::setOpacity(float opacity)
{
    return SDL_SetWindowOpacity(_get(),opacity);
}
float Window::getOpacity() const
{
    float op=-1;
    SDL_GetWindowOpacity(_get(),&op);
    return op;
}
#endif /// End of SDL2 2.0.5 Require.

/// FIXME: Not Implemented.
void Window::setResizable(bool resizable)
{
    //SDL_SetWindowResizable(_get(), resizable?SDL_TRUE:SDL_FALSE);
}

void Window::show()
{
    SDL_ShowWindow(_get());
}

void Window::hide()
{
    SDL_HideWindow(_get());
}

void Window::raise()
{
    SDL_RaiseWindow(_get());
}

void Window::minimize()
{
    SDL_MinimizeWindow(_get());
}

void Window::maximize()
{
    SDL_MaximizeWindow(_get());
}

void Window::restore()
{
    SDL_RestoreWindow(_get());
}

_DECL_DEPRECATED Surface Window::getSurface()
{
    SDL_Surface* temp = SDL_GetWindowSurface(_get());
    Surface s;
    /// Don't Free This Surface
    s._set_no_delete(temp);
    return s;
}

void Window::release()
{
    _clear();
}
#include "end_code.h"
