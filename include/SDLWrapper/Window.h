#pragma once
#include "include.h"
#include "WindowType.h"
#include "MessageBoxType.h"
#include "ErrorViewer.h"
#include "MessageBox.h"
#include "Surface.h"
#include "__Plugin.h"
namespace MiniEngine {
class Window
{
public:
    Window()=default;
    Window(std::string Title, int Width, int Height,
           std::initializer_list<WindowType> WindowFlags = {WindowType::Shown},
           int WindowPositionX=SDL_WINDOWPOS_CENTERED, int WindowPositionY=SDL_WINDOWPOS_CENTERED);

    Rect getSize() const;
    void setSize(const Rect& sizeRect);
    void setSize(int w, int h);

    Point getPosition() const;
    void setPosition(int x, int y);
    void setPosition(const Point& point);

    void setTitle(const std::string& Title);
    std::string getTitle() const;

    void setGrab(bool isGrab);
    bool getGrab() const;

#if _MINIENGINE_SDL_VERSION_ATLEAST(2,0,5)
    /// SDL2.0.5 Required.
    int setOpacity(float opacity);
    float getOpacity() const;
#endif

    /// FIXME: Not Implemented.
    void setResizable(bool resizable);

    /// Use UTF8 in Title and Message please.
    int showSimpleMessageBox(MessageBoxType type,const std::string& Title,const std::string& Message) const;

    int showMessageBox(const WindowMessageBox& box) const;

    void show();
    void hide();
    void raise();
    void minimize();
    void maximize();
    void restore();

    _DECL_DEPRECATED Surface getSurface();

    bool isScreenKeyboardShown();

    void release();
private:
    std::shared_ptr<SDL_Window> _wnd;

    void _set(SDL_Window*);
    void _clear();
    SDL_Window* _get() const;

    friend class Renderer;
    friend class _internal::Plugin;
};
} /// End of namespace MiniEngine
