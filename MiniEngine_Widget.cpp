#include "MiniEngine_Widget.h"
#include <iostream>
using namespace std;
using namespace MiniEngine;

namespace MiniEngine
{

namespace Widget
{

void Brush::setArea(Rect Area)
{
    area=Area;
}

int Brush::copy(Texture t,Rect src,Rect dst)
{
    dst.x+=area.x;
    dst.y+=area.y;

    if(dst.x>area.x+area.w||dst.y>area.y+area.h)
    {
        /// Not Printed.
        return 1;
    }

    if(dst.x+dst.w>area.x+area.w)
    {
        /// Some parts will not be printed.
        src.w=src.w*(area.w-dst.x+area.x)/dst.w;

        dst.w=area.w-(dst.x-area.x);
    }

    if(dst.y+dst.h>area.y+area.h)
    {
        /// Some parts will not be printed
        src.h=src.h*(area.h-dst.y+area.y)/dst.h;

        dst.h=area.h-(dst.y-area.y);
    }

    return Renderer::copy(t,src,dst);
}

int Brush::copyTo(Texture t,Rect dst)
{
    dst.x+=area.x;
    dst.y+=area.y;

    if(dst.x>area.x+area.w||dst.y>area.y+area.h)
    {
        /// Not Printed.
        return 1;
    }

    if(dst.x+dst.w>area.x+area.w||dst.y+dst.h>area.y+area.h)
    {
        /// Some parts will not be printed.
        Rect src=t.getSize();
        src.x=src.y=0;

        if(dst.x+dst.w>area.x+area.w)
        {
            src.w=src.w*(area.w-dst.x+area.x)/dst.w;

            dst.w=area.w-(dst.x-area.x);
        }

        if(dst.y+dst.h>area.y+area.h)
        {
            src.h=src.h*(area.h-dst.y+area.y)/dst.h;

            dst.h=area.h-(dst.y-area.y);
        }

        return Renderer::copy(t,src,dst);
    }
    else
    {
        return Renderer::copyTo(t,dst);
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

int Brush::copyFullFill(Texture t)
{
    Rect dst=area;
    return Renderer::copyTo(t,dst);
}

Brush::Brush(Renderer Rnd) : Renderer(Rnd)
{

}

Board::Board(Renderer Rnd,Rect Area) :  area(Area),brush(Rnd)
{
    brush.setArea(area);
}

Brush Board::getBrush()
{
    return brush;
}

Rect Board::getArea()
{
    return area;
}







ButtonBase::ButtonBase()
{
    status=1;
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

void ButtonBase::draw(Brush& brush) /// virtual
{
    int ret=-1;
    switch(status)
    {
    case 1:
        ret=brush.copyTo(t1,rect);
        break;
    case 2:
        ret=brush.copyTo(t2,rect);
        break;
    case 3:
        ret=brush.copyTo(t3,rect);
        break;
    }
}

int ButtonBase::handle(SDL_Event e,int& running,int& update) /// virtual
{
    switch(e.type)
    {
    case SDL_MOUSEMOTION:
        {
            if(Point(e.motion.x,e.motion.y).inRect(rect))
            {
                if(status==1)
                {
                    if(onmouseover) onmouseover();
                    status=2;
                    update=1;
                }
            }
            else
            {
                if(status==2)
                {
                    if(onmouseout) onmouseout();
                    status=1;
                    update=1;
                }
                else if(status==3)
                {
                    if(onrelease) onrelease();
                    if(onmouseout) onmouseout();
                    status=1;
                    update=1;
                }
            }

            return 1;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        {
            if(Point(e.button.x,e.button.y).inRect(rect))
            {
                if(status==2)
                {
                    if(onclicked) onclicked();
                    status=3;
                    update=1;
                }
            }

            return 1;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        {
            if(Point(e.button.x,e.button.y).inRect(rect))
            {
                if(status==3)
                {
                    if(onrelease) onrelease();
                    status=2;
                    update=1;
                }
            }

            return 1;
        }
        break;
    }

    return 0;
}

}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine::Widget
