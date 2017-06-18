#pragma once
#include "include.h"
#include "begin_code.h"
class Point
{
public:
    int x, y;
    Point(int X, int Y);
    Point();
    SDL_Point toSDLPoint() const;
    bool inRect(const Rect& rect) const;
};
#include "end_code.h"
