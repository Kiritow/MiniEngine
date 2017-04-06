#pragma once
#include "MiniEngine.h"
#include "MiniEngine_Event.h"

namespace MiniEngine
{

namespace Widget
{

class Brush : public Renderer
{
public:
    void setArea(Rect Area);
    void setFullArea(Rect FullArea);

    int copy(Texture t, Rect src, Rect dst);
    int copyTo(Texture t, Rect dst);
    int copyTo(Texture t, Point lupoint);
    int copyFill(Texture t,Rect src);
    int copyFullFill(Texture t);
protected:
    Brush(Renderer Rnd);
private:
    Rect area,fullarea;
    friend class Board;
};

class Drawable
{
public:
    virtual void draw(Brush& brush)=0;
    virtual ~Drawable()=default;
};

class ButtonBase : public Drawable, public EventHandlerBase
{
protected:
    bool onClick();
    bool onMouseOver();
    bool onMouseOut();

    /// Overrides
    bool onMouseDown();
    bool onMouseUp();
    bool onMouseMotion();
private:
    bool status;
};

}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine
