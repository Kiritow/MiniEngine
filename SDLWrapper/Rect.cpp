#include "Rect.h"

#include "begin_code.h"

Rect::Rect(int X, int Y, int W, int H)
{
    x = X;
    y = Y;
    w = W;
    h = H;
}

// explicit
Rect::Rect(const SDL_Rect& r):Rect(r.x,r.y,r.w,r.h)
{

}

Rect::Rect()
{
    x = y = w = h = 0;
}

SDL_Rect Rect::toSDLRect() const
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

bool Rect::isEmpty() const
{
    SDL_Rect r=toSDLRect();
    return SDL_RectEmpty(&r)==SDL_TRUE;
}

bool Rect::operator == (const Rect& r) const
{
    SDL_Rect a=toSDLRect(),b=r.toSDLRect();
    return SDL_RectEquals(&a,&b)==SDL_TRUE;
}

bool Rect::hasIntersection(const Rect& r) const
{
    SDL_Rect a=toSDLRect(),b=r.toSDLRect();
    return SDL_HasIntersection(&a,&b)==SDL_TRUE;
}

Rect Rect::getIntersection(const Rect& r) const
{
    SDL_Rect a=toSDLRect(),b=r.toSDLRect(),c;
    if(SDL_IntersectRect(&a,&b,&c)==SDL_TRUE)
    {
        return Rect(c);
    }
    else
    {
        return Rect();
    }
}

Rect Rect::getUnion(const Rect& r) const
{
    SDL_Rect a=toSDLRect(),b=r.toSDLRect(),c;
    SDL_UnionRect(&a,&b,&c);//void
    return Rect(c);
}

#include "end_code.h"
