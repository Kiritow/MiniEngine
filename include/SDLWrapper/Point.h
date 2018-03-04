#pragma once
#include "include.h"
#include "Rect.h"
namespace MiniEngine {

class Point
{
public:
    int x, y;
    Point(int X, int Y);
    Point();
    SDL_Point toSDLPoint() const;
    bool inRect(const Rect& rect) const;
};

} /// End of namespace MiniEngine
