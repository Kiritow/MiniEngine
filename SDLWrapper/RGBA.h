#pragma once
#include "include.h"
#include "ColorMode.h"
namespace MiniEngine
{

class RGBA
{
public:
    int r, g, b, a;
    RGBA(int R, int G, int B, int A);
    RGBA(ColorMode mode, int A);
    RGBA();
    SDL_Color toSDLColor() const;
    ColorMode toColorMode() const;
};

} /// End of namespace MiniEngine 

