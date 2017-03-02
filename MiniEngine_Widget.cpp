#include "MiniEngine_Widget.h"
using namespace MiniEngine;

void Brush::setArea(Rect Area)
{
    area=Area;
}

int Brush::copy(Texture t,Rect src,Rect dst)
{
    dst.x+=area.x;
    dst.y+=area.y;
    if(stretching)
    {
        if(dst.w>area.w)
        {
            dst.w=area.w;
        }
        if(dst.h>area.h)
        {
            dst.h=area.h;
        }
    }
    else
    {
        if(dst.w>area.w)
        {
            dst.w=src.w=area.w;
        }
        if(dst.h>area.h)
        {
            src.w=src.h=area.h;
        }
    }
    return Renderer::copy(t,src,dst);
}

int Brush::copyTo(Texture t,Rect dst)
{
    dst.x+=area.x;
    dst.y+=area.y;

    if(streching)
    {
        if(dst.w>area.w)
        {
            dst.w=area.w;
        }
        if(dst.h>area.h)
        {
            dst.h=area.h;
        }

        return Renderer::copyTo(t,dst);
    }
    else
    {
        Rect src=t.getSize();
        src.x=src.y=0;
        if(dst.w>area.w)
        {
            src.w=dst.w=area.w;
        }
        if(dst.h>area.h)
        {
            src.h=dst.h=area.h;
        }

        return Renderer::copy(t,src,dst);
    }
}

int Brush::copyTo(Texture t,Point lupoint)
{
    return copyTo(t,Rect(lupoint.x,lupoint.y,t.getw(),t.geth()));
}

int Brush::copyFill(Texture t,Rect src)
{
    Rect dst=area;
    return Renderer::copy(t,src,dst);
}

int Brush::copyFullFill(Texture t )
{
    Rect dst=area;
    return Renderer::copyTo(t,dst);
}

Board::Board(Window wnd,Rect Area)
{
    area=Area;
    fullwnd=wnd.getSize();
}

Brush Board::getBrush()
{
    Brush b;
    return b;
}

ButtonBase::ButtonBase()
{
    status=0;
}

void ButtonBase::setTextureClicked(Texture Clicked)
{
    t3=Clicked;
}

void ButtonBase::setTextureMouseover(Texture Mouseover)
{
    t2=Mouseover;
}

void ButtonBase::setTextureNormal(Texture Normal)
{
    t1=Normal;
}

void ButtonBase::setRect(Rect SensorArea)
{
    rect=SensorArea;
}

virtual void ButtonBase::draw(Brush& brush)
{
    switch(status)
    {
    case 0:
        brush.copyTo( )
    }
}
