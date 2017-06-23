#include "_caster.h"
#include "begin_code.h"
namespace _internal
{
BlendMode getBlendModeFromSDLBlendMode(SDL_BlendMode mode)
{
    switch(mode)
    {
    case SDL_BLENDMODE_ADD:
        return BlendMode::Add;
    case SDL_BLENDMODE_BLEND:
        return BlendMode::Blend;
    case SDL_BLENDMODE_MOD:
        return BlendMode::Mod;
    case SDL_BLENDMODE_NONE:
    default:/// return BlendMode::None on default.
        return BlendMode::None;
    }
}

SDL_BlendMode getSDLBlendModeFromBlendMode(BlendMode mode)
{
    switch(mode)
    {
    case BlendMode::Add:
        return SDL_BLENDMODE_ADD;
    case BlendMode::Blend:
        return SDL_BLENDMODE_BLEND;
    case BlendMode::Mod:
        return SDL_BLENDMODE_MOD;
    case BlendMode::None:
    default:/// return SDL_BLENDMODE_NONE on default.
        return SDL_BLENDMODE_NONE;
    }
}

/// FIXME: return SDL_WindowFlags or Uint32 ?
Uint32 getSDLWindowFlagsFromWindowType(WindowType type)
{
    switch(type)
    {
    case WindowType::FullScreen:
        return SDL_WINDOW_FULLSCREEN;
    case WindowType::OpenGL:
        return SDL_WINDOW_OPENGL;
    case WindowType::Shown:
        return SDL_WINDOW_SHOWN;
    case WindowType::Hidden:
        return SDL_WINDOW_HIDDEN;
    case WindowType::Borderless:
        return SDL_WINDOW_BORDERLESS;
    case WindowType::Resizable:
        return SDL_WINDOW_RESIZABLE;
    case WindowType::Minimized:
        return SDL_WINDOW_MINIMIZED;
    case WindowType::Maximized:
        return SDL_WINDOW_MAXIMIZED;
    case WindowType::InputGrabbed:
        return SDL_WINDOW_INPUT_GRABBED;
    case WindowType::InputFocus:
        return SDL_WINDOW_INPUT_FOCUS;
    case WindowType::MouseFocus:
        return SDL_WINDOW_MOUSE_FOCUS;
    case WindowType::FullScreenDesktop:
        return SDL_WINDOW_FULLSCREEN_DESKTOP;
    case WindowType::Foreign:
        return SDL_WINDOW_FOREIGN;
    case WindowType::AllowHighDPI:
        return SDL_WINDOW_ALLOW_HIGHDPI;
    case WindowType::MouseCapture:
        return SDL_WINDOW_MOUSE_CAPTURE;

#if _MINIENGINE_SDL_VERSION_ATLEAST(2,0,5) /// SDL 2.0.5 Required
    case WindowType::AlwaysOnTop:
        return SDL_WINDOW_ALWAYS_ON_TOP;
    case WindowType::SkipTaskBar:
        return SDL_WINDOW_SKIP_TASKBAR;
    case WindowType::Utility:
        return SDL_WINDOW_UTILITY;
    case WindowType::ToolTip:
        return SDL_WINDOW_TOOLTIP;
    case WindowType::PopUpMenu:
        return SDL_WINDOW_POPUP_MENU;
#endif // End of SDL2.0.5 Require

    default:
        return 0;/// Return 0 on default.
    }
}

SystemCursorType getCursorTypeFromSDLSystemCursor(SDL_SystemCursor id)
{
    switch(id)
    {
    case SDL_SYSTEM_CURSOR_ARROW:
        return SystemCursorType::Arrow;
    case SDL_SYSTEM_CURSOR_CROSSHAIR:
        return SystemCursorType::CrossHair;
    case SDL_SYSTEM_CURSOR_HAND:
        return SystemCursorType::Hand;
    case SDL_SYSTEM_CURSOR_IBEAM:
        return SystemCursorType::Ibeam;
    case SDL_SYSTEM_CURSOR_NO:
        return SystemCursorType::No;
    case SDL_SYSTEM_CURSOR_SIZEALL:
        return SystemCursorType::SizeAll;
    case SDL_SYSTEM_CURSOR_SIZENESW:
        return SystemCursorType::SizeNESW;
    case SDL_SYSTEM_CURSOR_SIZENS:
        return SystemCursorType::SizeNS;
    case SDL_SYSTEM_CURSOR_SIZENWSE:
        return SystemCursorType::SizeNWSE;
    case SDL_SYSTEM_CURSOR_SIZEWE:
        return SystemCursorType::SizeWE;
    case SDL_SYSTEM_CURSOR_WAIT:
        return SystemCursorType::Wait;
    case SDL_SYSTEM_CURSOR_WAITARROW:
        return SystemCursorType::WaitArrow;
    default:/// return SystemCursorType::Arrow on default.
        return SystemCursorType::Arrow;
    }
}

SDL_SystemCursor getSDLSystemCursorFromSystemCursorType(SystemCursorType type)
{
    switch(type)
    {
    case SystemCursorType::Arrow:
        return SDL_SYSTEM_CURSOR_ARROW;
    case SystemCursorType::CrossHair:
        return SDL_SYSTEM_CURSOR_CROSSHAIR;
    case SystemCursorType::Hand:
        return SDL_SYSTEM_CURSOR_HAND;
    case SystemCursorType::Ibeam:
        return SDL_SYSTEM_CURSOR_IBEAM;
    case SystemCursorType::No:
        return SDL_SYSTEM_CURSOR_NO;
    case SystemCursorType::SizeAll:
        return SDL_SYSTEM_CURSOR_SIZEALL;
    case SystemCursorType::SizeNESW:
        return SDL_SYSTEM_CURSOR_SIZENESW;
    case SystemCursorType::SizeNS:
        return SDL_SYSTEM_CURSOR_SIZENS;
    case SystemCursorType::SizeNWSE:
        return SDL_SYSTEM_CURSOR_SIZENWSE;
    case SystemCursorType::SizeWE:
        return SDL_SYSTEM_CURSOR_SIZEWE;
    case SystemCursorType::Wait:
        return SDL_SYSTEM_CURSOR_WAIT;
    case SystemCursorType::WaitArrow:
        return SDL_SYSTEM_CURSOR_WAITARROW;
    default:/// return SDL_SYSTEM_CURSOR_ARROW on default.
        return SDL_SYSTEM_CURSOR_ARROW;
    }
}

int getTTFFontStyleFromFontStyle(FontStyle style)
{
    switch(style)
    {
    case FontStyle::Bold:
        return TTF_STYLE_BOLD;
    case FontStyle::Italic:
        return TTF_STYLE_ITALIC;
    case FontStyle::Normal:
        return TTF_STYLE_NORMAL;
    case FontStyle::StrikeThrough:
        return TTF_STYLE_STRIKETHROUGH;
    case FontStyle::UnderLine:
        return TTF_STYLE_UNDERLINE;
    default:
        return TTF_STYLE_NORMAL;
    }
}

std::vector<FontStyle> getFontStyleVecFromMixedTTFFontStyle(int Mixed_TTF_Font_Style)
{
    std::vector<FontStyle> vec;
    if(Mixed_TTF_Font_Style&TTF_STYLE_BOLD)
    {
        vec.push_back(FontStyle::Bold);
    }
    if(Mixed_TTF_Font_Style&TTF_STYLE_ITALIC)
    {
        vec.push_back(FontStyle::Italic);
    }
    if(Mixed_TTF_Font_Style&TTF_STYLE_STRIKETHROUGH)
    {
        vec.push_back(FontStyle::StrikeThrough);
    }
    if(Mixed_TTF_Font_Style&TTF_STYLE_UNDERLINE)
    {
        vec.push_back(FontStyle::UnderLine);
    }
    if(vec.empty())
    {
        vec.push_back(FontStyle::Normal);
    }

    return vec;
}

SDL_RendererFlip getSDLRendererFlipFromFlipMode(FlipMode mode)
{
    switch(mode)
    {
    case FlipMode::None:
        return SDL_FLIP_NONE;
    case FlipMode::Horizontal:
        return SDL_FLIP_HORIZONTAL;
    case FlipMode::Vertical:
        return SDL_FLIP_VERTICAL;
    default:
        /// return non-flip mode on default.
        return SDL_FLIP_NONE;
    }
}
}/// End of namespace _internal
#include "end_code.h"