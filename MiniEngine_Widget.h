#pragma once
#include "MiniEngine.h"
#include <functional>

class Brush : public MiniEngine::Renderer
{
public:
    void setArea(MiniEngine::Rect Area);

    int copy(MiniEngine::Texture t, MiniEngine::Rect src, MiniEngine::Rect dst);
    int copyTo(MiniEngine::Texture t, MiniEngine::Rect dst);
    int copyTo(MiniEngine::Texture t, MiniEngine::Point lupoint);
    int copyFill(MiniEngine::Texture t,MiniEngine::Rect src);
    int copyFullFill(MiniEngine::Texture t);
private:
    MiniEngine::Rect area;
};

class Board
{
public:
    Board(MiniEngine::Window wnd,MiniEngine::Rect Area);
    Brush getBrush();
private:
    MiniEngine::Rect area,fullwnd;
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
    void setTextureNormal(MiniEngine::Texture Normal);
    void setTextureMouseover(MiniEngine::Texture Mouseover);
    void setTextureClicked(MiniEngine::Texture Clicked);
    void setRect(MiniEngine::Rect SensorArea);
    virtual void draw(Brush& brush);
    virtual void handle(SDL_Event e,int& running,int& update);
private:
    int status;
    MiniEngine::Texture t1,t2,t3;
    MiniEngine::Rect rect;
};
