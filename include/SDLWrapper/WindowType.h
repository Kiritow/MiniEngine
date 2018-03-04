#pragma once
namespace MiniEngine {
enum class WindowType
{
    FullScreen, OpenGL, Shown, Hidden,
    Borderless, Resizable, Minimized, Maximized,
    InputGrabbed, InputFocus, MouseFocus,
    FullScreenDesktop, Foreign, AllowHighDPI,
    MouseCapture, AlwaysOnTop, SkipTaskBar,
    Utility, ToolTip, PopUpMenu
};
} /// End of namespace MiniEngine
