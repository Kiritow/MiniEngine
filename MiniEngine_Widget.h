#pragma once
#include "MiniEngine.h"
#include "MiniEngine_Event.h"
#include <string>
#include <list>

namespace MiniEngine
{

namespace Widget
{

class PosInfo
{
public:
    PosInfo() : x(0),y(0),w(1),h(1){}
    PosInfo(double X,double Y,double W,double H) : x(X),y(Y),w(W),h(H) {}
    double x,y;
    double w,h;
    Rect getRect(Rect Area);
};

class PointInfo
{
public:
    double x,y;
};


/// Forward Declaration
class Frame;
class Board;

class Brush : public Renderer
{
public:
    int copy(Texture t, Rect src, PosInfo dst);
    int copyTo(Texture t, PosInfo dst);
    int copyTo(Texture t, PointInfo lupoint);
    int copyFill(Texture t,Rect src);
    int copyFullFill(Texture t);
    Rect getArea();
    void setArea(Rect Area);
    int fillRect(PosInfo info);
protected:
    Brush(Renderer Rnd);
private:
    Rect area;
    friend class Frame;
};

class _WidgetEventBase : public MiniEngine::EventSystem::EventBase
{
public:
    SDL_Event getRealEvent();
};

class _PositionEvent : public _WidgetEventBase
{
public:
    double x,y;
    void updatePos(int X,int Y,Rect);
};

using MouseButton = MiniEngine::EventSystem::MouseButton;

class MouseButtonEvent : public _PositionEvent
{
public:
    MouseButton button;
    void update(const MiniEngine::EventSystem::MouseButtonEvent&,Rect);
};

class MouseMotionEvent : public _PositionEvent
{
public:
    void update(const MiniEngine::EventSystem::MouseMotionEvent&,Rect);
};

class KeyEvent : public _WidgetEventBase
{
public:
    int key;
    void update(const MiniEngine::EventSystem::KeyEvent&);
};


class BoardBase : public MiniEngine::EventSystem::EventHandlerBase
{
public:
    virtual void draw(Brush&)=0;
    PosInfo info;
    Rect _realrect;

    BoardBase* getParent();
    virtual void setParent(BoardBase*);
    Frame* getFrame();
    virtual void setFrame(Frame*);

protected:
    virtual bool onMouseDown(const MouseButtonEvent&);
    virtual bool onMouseUp(const MouseButtonEvent&);
    virtual bool onMouseMotion(const MouseMotionEvent&);
    virtual bool onKeyDown(const KeyEvent&);
    virtual bool onKeyUp(const KeyEvent&);
    BoardBase* _parent;
    Frame* _frame;
private:
    virtual bool onMouseDown(const MiniEngine::EventSystem::MouseButtonEvent&) override;
    virtual bool onMouseUp(const MiniEngine::EventSystem::MouseButtonEvent&) override;
    virtual bool onMouseMotion(const MiniEngine::EventSystem::MouseMotionEvent&) override;
    virtual bool onKeyDown(const MiniEngine::EventSystem::KeyEvent&) override;
    virtual bool onKeyUp(const MiniEngine::EventSystem::KeyEvent&) override;
};

class Frame
{
public:
    Frame(Renderer,Rect);
    void run();
    void stop();
    void needUpdate();
    void add(Board*);
    int remove(Board*);
private:
    bool running;
    bool update;
    SDL_Event e;
    std::list<Board*> _lst;
    Brush brush;
};

class WidgetBase : public BoardBase
{
public:
    Board* getBoard();
private:
    friend class Board;
};

class Board : public BoardBase
{
public:
    Board();
    void add(BoardBase*);
    bool remove(BoardBase*);
    virtual void draw(Brush&) override;
    virtual bool event(const MiniEngine::EventSystem::EventBase& ev) override;

    virtual void setParent(BoardBase*) override;
    virtual void setFrame(Frame*) override;
private:
    std::list<BoardBase*> _lst;
};

class ButtonBase : public WidgetBase
{
protected:
    ButtonBase();
    virtual void onRelease();
    virtual void onClick();
    virtual void onMouseOver();
    virtual void onMouseOut();
    /// Button Status: 0 Normal 1 MouseOver 2 MouseDown(Clicked)
    int _status;
private:
    /// Overrides, called by EventHandler::event()
    virtual bool onMouseDown(const MouseButtonEvent&) override;
    virtual bool onMouseUp(const MouseButtonEvent&) override;
    virtual bool onMouseMotion(const MouseMotionEvent&) override;
};

class TextButton : public ButtonBase
{
public:
    void setText(std::string Text);
    std::string getText();
    virtual void draw(Brush&) override;
private:
    std::string _word;
    Texture _text;
};

class ColorButton : public ButtonBase
{
public:
    RGBA normal,active,clicked;
    virtual void draw(Brush&) override;
protected:
    virtual void onRelease() override;
    virtual void onClick() override;
    virtual void onMouseOver() override;
    virtual void onMouseOut() override;
};

class ProcessBarBase : public WidgetBase
{
public:
    int currentVal,maxVal;
    ProcessBarBase();
    virtual void draw(Brush&) override;
};

}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine
