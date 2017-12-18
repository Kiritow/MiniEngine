#include "Point.h"
#include "begin_code.h"

Point::Point(int X, int Y)
{
    x = X;
    y = Y;
}

Point::Point()
{
    x = y = 0;
}

SDL_Point Point::toSDLPoint() const
{
    SDL_Point p;
    p.x = x;
    p.y = y;
    return p;
}

bool Point::inRect(const Rect& rect) const
{
    auto p = toSDLPoint();
    auto r = rect.toSDLRect();
    return ( SDL_PointInRect(&p, &r) == SDL_TRUE );
}

#include "end_code.h"
