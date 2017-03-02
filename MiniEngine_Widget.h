#pragma once
#include "MiniEngine.h"
#include <functional>

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
private:
    Rect area,fullarea;
};

class Board
{
public:
    Board(Rect Area);
    Brush getBrush();
    Rect getArea();
private:
    Rect area;
};

class Drawable
{
public:
    virtual void draw(Brush& brush)=0;
    virtual ~Drawable()=default;
};

class Interactive
{
public:
    virtual void handle(SDL_Event e,int& running,int& update)=0;
};

class ButtonBase : public Drawable, public Interactive
{
public:
    ButtonBase();
    void setTextureNormal(Texture Normal);
    void setTextureMouseover(Texture Mouseover);
    void setTextureClicked(Texture Clicked);
    void setRect(Rect SensorArea);
    virtual void draw(Brush& brush);
    virtual void handle(SDL_Event e,int& running,int& update);
private:
    int status;
    Texture t1,t2,t3;
    Rect rect;
};

}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine
