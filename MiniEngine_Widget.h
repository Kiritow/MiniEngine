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


class BoardBase : public EventHandlerBase
{
public:
    virtual void draw(Brush&)=0;
    PosInfo info;
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
    PosInfo info;
private:
    Board* _parent;
    friend class Board;
};

class Board : public BoardBase
{
public:
    void add(Board*);
    void add(WidgetBase*);
    int remove(Board*);
    int remove(WidgetBase*);
    virtual void draw(Brush&);
    virtual bool event(const EventBase& ev) override;
    Board* getParent();
    Frame* getFrame();
    PosInfo getPosInfo();
private:
    std::list<Board*> _blst;
    std::list<WidgetBase*> _wlst;

    Board* _parent;
    Frame* _frame;
    friend class Frame;
};



class ButtonBase : public WidgetBase
{
protected:
    ButtonBase();
    void onPressed();
    void onClick();
    void onMouseOver();
    void onMouseOut();
private:
    /// Overrides, called by EventHandler::event()
    virtual bool onMouseDown(const MouseButtonEvent&) override;
    virtual bool onMouseUp(const MouseButtonEvent&) override;
    virtual bool onMouseMotion(const MouseMotionEvent&) override;

    int _status;
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
    void onPressed();
    void onClick();
    void onMouseOver();
    void onMouseOut();
private:
    int _colorstatus;
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
