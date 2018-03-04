#pragma once
#include "include.h"

namespace MiniEngine {

class Rect
{
public:
    int x, y, w, h;
    Rect(int X, int Y, int W, int H);
    explicit Rect(const SDL_Rect&);
    Rect();
    SDL_Rect toSDLRect() const;
    bool isEmpty() const;
    bool operator == (const Rect&) const;
    bool hasIntersection(const Rect&) const;
    Rect getIntersection(const Rect&) const;
    Rect getUnion(const Rect&) const;
};

} /// End of namespace MiniEngine
