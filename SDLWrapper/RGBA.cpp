#include "RGBA.h"
namespace MiniEngine
{
RGBA::RGBA(int R, int G, int B, int A)
{
    r = R;
    g = G;
    b = B;
    a = A;
}

RGBA::RGBA(ColorMode mode, int A)
{
    r = mode.r;
    g = mode.g;
    b = mode.b;
    a = A;
}

RGBA::RGBA()
{
    r = g = b = a = 0;
}

SDL_Color RGBA::toSDLColor() const
{
    SDL_Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

ColorMode RGBA::toColorMode() const
{
    return ColorMode(r, g, b);
}
} /// End of namespace MiniEngine 

