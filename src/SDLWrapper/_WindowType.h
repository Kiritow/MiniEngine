#pragma once
#include "begin_code.h"
enum class WindowType
{
    FullScreen, OpenGL, Shown, Hidden,
    Borderless, Resizable, Minimized, Maximized,
    InputGrabbed, InputFocus, MouseFocus,
    FullScreenDesktop, Foreign, AllowHighDPI,
    MouseCapture, AlwaysOnTop, SkipTaskBar,
    Utility, ToolTip, PopUpMenu
};
#include "end_code.h"
