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
    double x,y;
    double w,h;
    Rect getRect(Rect SizeRect);
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
protected:
    Brush(Renderer Rnd);
    void setArea(Rect Area);
private:
    Rect area;
    friend class Frame;
};


class BoardBase : public EventHandlerBase
{
public:
    virtual void draw(const Brush&)=0;
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

class Board : public BoardBase
{
public:
    void add(BoardBase*);
    int remove(BoardBase*);
    virtual void draw(const Brush&);
    virtual bool event(const EventBase& ev) override;
    Frame* getFrame();
    PosInfo getPosInfo();
private:
    std::list<BoardBase*> _lst;
    PosInfo info;
};

class WidgetBase : public BoardBase
{
public:
    Board* getBoard();
protected:
    PosInfo info;
};

class ButtonBase : public WidgetBase
{
protected:
    bool onClick();
    bool onMouseOver();
    bool onMouseOut();
private:
    /// Overrides
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
    virtual void draw(const Brush&) override;
private:
    std::string _word;
    Texture _text;
};

}/// End of namespace MiniEngine::Widget

}/// End of namespace MiniEngine
