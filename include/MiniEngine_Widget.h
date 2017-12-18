#pragma once
#include "MiniEngine.h"
#include <functional>
#include <vector>

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

class Interactive
{
public:
    virtual int handle(SDL_Event e,int& running,int& update)=0;
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
    virtual int handle(SDL_Event e,int& running,int& update);

    std::function<void()> onmouseover,onclicked,onrelease,onmouseout;
private:
    int status;
    Texture t1,t2,t3;
    Rect rect;
};

class Board
{
public:
    Board(Renderer Rnd,Rect Area);
    Brush getBrush();
    Rect getArea();

    class _Control
    {
    public:
        void add(Interactive* widget);
        Interactive* at(int index);
        int size();
        bool remove(Interactive* widget);
    private:
        std::vector<Interactive*> vec;
    }Control;

private:
    Rect area;
    Brush brush;
};

}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine
