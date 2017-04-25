#include "MiniEngine_Widget.h"
#include <algorithm>

namespace MiniEngine
{

namespace Widget
{

Rect PosInfo::getRect(Rect Area)
{
    Rect NewArea;
    NewArea.x=Area.x+Area.w*x;
    NewArea.y=Area.y+Area.h*y;
    NewArea.w=Area.w*w;
    NewArea.h=Area.h*h;
    return NewArea;
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


SDL_Event _WidgetEventBase::getRealEvent()
{
    return e;
}

void _PositionEvent::updatePos(int X,int Y,Rect Area)
{
    printf("PositionEvent::updatePos(%d,%d,Rect:%d %d %d %d)\n",X,Y,Area.x,Area.y,Area.w,Area.h);
    x=(double)(X-Area.x)/Area.w;
    y=(double)(Y-Area.y)/Area.h;
    printf("Position: %.2f %.2f\n",x,y);
}

void MouseButtonEvent::update(const MiniEngine::EventSystem::MouseButtonEvent& ev,Rect Area)
{
    updatePos(ev.x,ev.y,Area);
    button=ev.button;
}

void MouseMotionEvent::update(const MiniEngine::EventSystem::MouseMotionEvent& ev,Rect Area)
{
    updatePos(ev.x,ev.y,Area);
}

void KeyEvent::update(const MiniEngine::EventSystem::KeyEvent& ev)
{
    key=ev.key;
}

Frame::Frame(Renderer rnd,Rect Area) : brush(rnd)
{
    brush.setArea(Area);
    update=running=true;
}

void Frame::add(Board* p)
{
    _lst.push_back(p);
    p->setFrame(this);
}

int Frame::remove(Board* p)
{
    _lst.remove(p);
    p->setFrame(nullptr);
    return 0;
}

void Frame::run()
{
    while(running)
    {
        while(!update&&SDL_WaitEvent(&e))
        {
            MiniEngine::EventSystem::EventBase x(e);
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
                (*iter)->_realrect=(*iter)->info.getRect(brush.getArea());
                (*iter)->draw(brush);
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

//private virtual override
bool BoardBase::onMouseDown(const MiniEngine::EventSystem::MouseButtonEvent& ev)
{
    MouseButtonEvent e;
    e.update(ev,_realrect);
    return onMouseDown(e);
}
//private virtual override
bool BoardBase::onMouseUp(const MiniEngine::EventSystem::MouseButtonEvent& ev)
{
    MouseButtonEvent e;
    e.update(ev,_realrect);
    return onMouseUp(e);
}
//private virtual override
bool BoardBase::onMouseMotion(const MiniEngine::EventSystem::MouseMotionEvent& ev)
{
    MouseMotionEvent e;
    e.update(ev,_realrect);
    return onMouseMotion(e);
}
//private virtual override
bool BoardBase::onKeyDown(const MiniEngine::EventSystem::KeyEvent& ev)
{
    KeyEvent e;
    e.update(ev);
    return onKeyDown(e);
}
//private virtual override
bool BoardBase::onKeyUp(const MiniEngine::EventSystem::KeyEvent& ev)
{
    KeyEvent e;
    e.update(ev);
    return onKeyUp(e);
}
//protected virtual
bool BoardBase::onMouseDown(const MouseButtonEvent& ev)
{
    return false;
}
//protected virtual
bool BoardBase::onMouseUp(const MouseButtonEvent& ev)
{
    return false;
}
//protected virtual
bool BoardBase::onMouseMotion(const MouseMotionEvent& ev)
{
    return false;
}
//protected virtual
bool BoardBase::onKeyDown(const KeyEvent& ev)
{
    return false;
}
//protected virtual
bool BoardBase::onKeyUp(const KeyEvent& ev)
{
    return false;
}
Frame* BoardBase::getFrame()
{
    return _frame;
}
BoardBase* BoardBase::getParent()
{
    return _parent;
}
//virtual
void BoardBase::setFrame(Frame* pFrame)
{
    _frame=pFrame;
}
//virtual
void BoardBase::setParent(BoardBase* pBoardBase)
{
    _parent=pBoardBase;
}

Board::Board()
{
    setParent(nullptr);
    setFrame(nullptr);
}

void Board::add(BoardBase* p)
{
    _lst.push_back(p);
    p->setParent(this);
    p->setFrame(getFrame());
}

bool Board::remove(BoardBase* p)
{
    if(std::find(_lst.begin(),_lst.end(),p)!=_lst.end())
    {
        _lst.remove(p);
        p->setParent(nullptr);
        p->setFrame(nullptr);
        return true;
    }
    return false;
}

void Board::draw(Brush& b)
{
    Brush nb=b;
    nb.setArea(_realrect);
    for(auto& p:_lst)
    {
        p->_realrect=p->info.getRect(_realrect);
        p->draw(nb);
    }
}

bool Board::event(const MiniEngine::EventSystem::EventBase& ev) /// virtual override
{
    for(auto& p:_lst)
    {
        if(p->event(ev)) return true;
    }
    return false;
}
//virtual override
void Board::setFrame(Frame* pFrame)
{
    _frame=pFrame;
    for(auto& p:_lst)
    {
        p->setFrame(pFrame);
    }
}
//virtual override
void Board::setParent(BoardBase* pBoardBase)
{
    _parent=pBoardBase;
    for(auto& p:_lst)
    {
        p->setParent(pBoardBase);
    }
}

ButtonBase::ButtonBase()
{
    _status=0;
}

void ButtonBase::onRelease()
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
//private override
bool ButtonBase::onMouseDown(const MouseButtonEvent& ev)
{
    if(_status==1&&ev.x>0&&ev.y>0&&ev.x<1&&ev.y<1)
    {
        _status=2;
        onClick();
        return true;
    }
    return false;
}
//private override
bool ButtonBase::onMouseUp(const MouseButtonEvent& ev)
{
    if(_status==2&&ev.x>0&&ev.y>0&&ev.x<1&&ev.y<1)
    {
        _status=1;
        onRelease();
        return true;
    }
    return false;
}
//private override
bool ButtonBase::onMouseMotion(const MouseMotionEvent& ev)
{
    if(_status==0&&ev.x>0&&ev.y>0&&ev.x<1&&ev.y<1)
    {
        _status=1;
        onMouseOver();
        return true;
    }
    else if(_status==1&&!(ev.x>0&&ev.y>0&&ev.x<1&&ev.y<1))
    {
        _status=0;
        onMouseOut();
        return true;
    }
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

void ColorButton::onRelease()
{
    getFrame()->needUpdate();
    printf("Released\n");
}
void ColorButton::onClick()
{
    getFrame()->needUpdate();
    printf("Clicked\n");
}
void ColorButton::onMouseOver()
{
    getFrame()->needUpdate();
    printf("MouseOver\n");
}
void ColorButton::onMouseOut()
{
    getFrame()->needUpdate();
    printf("MouseOut\n");
}
void ColorButton::draw(Brush& b)
{
    RGBA td=b.getColor();
    switch(_status)
    {
    case 0:/// Normal
        b.setColor(normal);
        b.fillRect(info);
        break;
    case 1:/// Active (MouseOver)
        b.setColor(active);
        b.fillRect(info);
        break;
    case 2:/// Pressed Down (MouseUp)
        b.setColor(clicked);
        b.fillRect(info);
        break;
    }
    b.setColor(td);
}


}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine::Widget
