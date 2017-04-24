#include "MiniEngine_Widget.h"

namespace MiniEngine
{

namespace Widget
{

Rect PosInfo::getRect(Rect Area)
{
    Area.x*=x;
    Area.y*=y;
    Area.w*=w;
    Area.h*=h;
    return Area;
}


void Brush::setArea(Rect Area)
{
    area=Area;
}

Rect Brush::getArea()
{
    return area;
}

int Brush::copy(Texture t,Rect src,PosInfo dst_posinfo)
{
    Rect dst=dst_posinfo.getRect(area);
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

int Brush::copyTo(Texture t,PosInfo dst_posinfo)
{
    Rect dst=dst_posinfo.getRect(area);
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

int Brush::copyTo(Texture t,PointInfo lupoint)
{
    PosInfo pos;
    pos.x=lupoint.x;
    pos.y=lupoint.y;
    pos.w=(double)t.getw()/area.w;
    pos.h=(double)t.geth()/area.h;
    return copyTo(t,pos);
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

int Brush::fillRect(PosInfo info)
{
    return Renderer::fillRect(info.getRect(area));
}




Frame::Frame(Renderer rnd,Rect Area) : brush(rnd)
{
    brush.setArea(Area);
    update=running=true;
}

void Frame::add(Board* p)
{
    _lst.push_back(p);
    p->_frame=this;
}

int Frame::remove(Board* p)
{
    _lst.remove(p);
    p->_frame=nullptr;
    return 0;
}

void Frame::run()
{
    while(running)
    {
        while(!update&&SDL_WaitEvent(&e))
        {
            EventBase x(e);
            bool dealed=false;
            if(!_lst.empty())
            {
                for(auto iter=_lst.rbegin();iter!=_lst.rend();++iter)
                {
                    if((dealed=(*iter)->event(x))) break;
                }
            }

            /// If Message Not Solved, Try to Solve it here. (Maybe System Message)
            if(!dealed)
            {
                switch(e.type)
                {
                case SDL_QUIT:
                    stop();
                    needUpdate();
                    break;
                }
            }
        }

        brush.clear();

        if(!_lst.empty())
        {
            for(auto iter=_lst.rbegin();iter!=_lst.rend();++iter)
            {
                Brush nbrush=brush;
                nbrush.setArea((*iter)->getPosInfo().getRect(brush.getArea()));
                (*iter)->draw(nbrush);
            }
        }

        brush.update();

        update=false;

        printf("Looped\n");
    }
}

void Frame::stop()
{
    running=false;
}

void Frame::needUpdate()
{
    update=true;
}





void Board::add(Board* p)
{
    _blst.push_back(p);
    p->_parent=this;
}

int Board::remove(Board* p)
{
    _blst.remove(p);
    p->_parent=nullptr;
    return 0;
}

void Board::add(WidgetBase* p)
{
    _wlst.push_back(p);
    p->_parent=this;
}

int Board::remove(WidgetBase* p)
{
    _wlst.remove(p);
    p->_parent=nullptr;
    return 0;
}

PosInfo Board::getPosInfo()
{
    return info;
}

void Board::draw(Brush& b)
{
    /// FIXME: Bug Found while trying to draw a Board in Board.
    for(auto& p:_wlst)
    {
        p->draw(b);
    }

    Brush nb=b;
    nb.setArea(info.getRect(nb.getArea()));
    for(auto& p:_blst)
    {
        p->draw(nb);
    }
}

bool Board::event(const EventBase& ev) /// virtual override
{
    for(auto& p:_wlst)
    {
        if(p->event(ev)) return true;
    }

    for(auto& p:_blst)
    {
        if(p->event(ev)) return true;
    }

    return false;
}

ButtonBase::ButtonBase()
{
    _status=0;
}

void ButtonBase::onPressed()
{

}

void ButtonBase::onClick()
{

}

void ButtonBase::onMouseOver()
{

}

void ButtonBase::onMouseOut()
{

}

bool ButtonBase::onMouseDown(const MouseButtonEvent& ev)
{

    return false;
}

bool ButtonBase::onMouseUp(const MouseButtonEvent& ev)
{
    return false;
}

bool ButtonBase::onMouseMotion(const MouseMotionEvent& ev)
{
    return false;
}

void TextButton::setText(std::string Text)
{
    if(Text==_word) return;
    _word=Text;
    /// TODO: How to Render...
}

std::string TextButton::getText()
{
    return _word;
}

void TextButton::draw(Brush&)
{
    /// FIXME: Unfinished TextButton::draw() due to Font loading in Frame.
    printf("TextButton::draw()\n");
}

void ColorButton::draw(Brush& b)
{
    RGBA td=b.getColor();
    switch(_colorstatus)
    {
    case 0:/// Normal
        /// How to fill rect with PosInfo and Brush...
        b.setColor(normal);
        b.fillRect(info);
        break;
    case 1:/// Active (MouseOver)
        b.setColor(active);
        b.fillRect(info);
        break;
    case 2:/// Pressed Down (MouseUp)
        b.setColor(active);
        b.fillRect(info);
        break;
    }
    b.setColor(td);
}


}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine::Widget
