#include "MiniEngine.h"
#include <functional>
#include <vector>
#include <map>
#include <mutex>
using namespace MiniEngine;


using DispatcherType = function<void(SDL_Event e,int& running,int& update)> ;
map<int,DispatcherType> disvec;

mutex mdisvec_counter;
int disvec_counter=0;



using UpdaterType = function<void(Renderer& rnd)> ;
map<int,UpdaterType> upvec;

mutex mupvec_counter;
int upvec_counter=0;

int RegistDispatcher(DispatcherType func)
{
    mdisvec_counter.lock();
    int id=disvec_counter++;
    mdisvec_counter.unlock();

    disvec.insert(make_pair(id,func));
    return id;
}
int UnregistDispatcher(int callbackid)
{
    for(auto it=disvec.begin();it!=disvec.end();it++)
    {
        if(callbackid==it->first)
        {
            disvec.erase(it);
            return 0;
        }
    }
    return -1;
}
int RegistUpdater(UpdaterType func)
{
    mupvec_counter.lock();
    int id=upvec_counter++;
    mupvec_counter.unlock();

    upvec.insert(make_pair(id,func));
    return id;
}
int UnregistUpdater(int callbackid)
{
    for(auto it=upvec.begin();it!=upvec.end();it++)
    {
        if(callbackid==it->first)
        {
            upvec.erase(it);
            return 0;
        }
    }
    return -1;
}

void EventDispatcher(SDL_Event e,int& running,int& update)
{
    for(auto& func:disvec)
    {
        int r=1;
        int u=0;
        func.second(e,r,u);
        running&=r;
        update|=u;
    }
}

void EventUpdater(Renderer& rnd)
{
    for(auto& func:upvec)
    {
        func.second(rnd);
    }
}

void EventLoop(Renderer& rnd)
{
    SDL_Event e;
    int running=1;
    int update=1;
    while(running)
    {
        while(!update&&SDL_WaitEvent(&e))
        {
            EventDispatcher(e,running,update);
        }

        rnd.clear();
        EventUpdater(rnd);
        rnd.update();
        update=0;
    }
}

class Drawable
{
public:
    virtual void update(Renderer& rnd)=0;
    virtual ~Drawable()
    {

    }
};

class Interactive : public Drawable
{
public:
    virtual void handle(SDL_Event e,int& running,int& update)=0;
};

class SimpleButton : public Interactive
{
public:
    enum class ButtonStatus {normal,mouseover,clicked};

    /// StatusChanged API
    function<void(int& running,int& update,ButtonStatus& currentStatus,ButtonStatus newStatus)> onStatusChanged;


    virtual ~SimpleButton()
    {
        /// Disable the button while destructing
        disable();
    }

    SimpleButton(Rect rect)
    {
        r=rect;
        bstatus=ButtonStatus::normal;
    }
    void setNormalColor(RGBA NormalColor)
    {
        normal=NormalColor;
    }
    void setMouseoverColor(RGBA MouseoverColor)
    {
        mouseover=MouseoverColor;
    }
    void setClickedColor(RGBA ClickedColor)
    {
        clicked=ClickedColor;
    }

    void enable()
    {
        disid=RegistDispatcher([this](SDL_Event e,int& r,int& u){handle(e,r,u);});
        upid=RegistUpdater([this](Renderer& rnd){update(rnd);});
    }
    void disable()
    {
        UnregistUpdater(upid);
        UnregistDispatcher(disid);
    }

    virtual void update(Renderer& rnd) override
    {
        RGBA old=rnd.getColor();
        switch(bstatus)
    {
        case ButtonStatus::normal:rnd.setColor(normal);break;
        case ButtonStatus::mouseover:rnd.setColor(mouseover);break;
        case ButtonStatus::clicked:rnd.setColor(clicked);break;
        }
        rnd.fillRect(r);
        rnd.setColor(old);
    }
    virtual void handle(SDL_Event e,int& running,int& update) override
    {
        switch(e.type)
        {
        case SDL_MOUSEMOTION:
            {
                if(Point(e.motion.x,e.motion.y).inRect(r))
                {
                    if(bstatus!=ButtonStatus::mouseover)
                    {
                        if(onStatusChanged)
                            onStatusChanged(running,update,bstatus,ButtonStatus::mouseover);
                        else
                        {
                            bstatus=ButtonStatus::mouseover;
                            update=1;
                        }
                    }
                }
                else
                {
                    if(bstatus!=ButtonStatus::normal)
                    {
                        if(onStatusChanged)
                            onStatusChanged(running,update,bstatus,ButtonStatus::normal);
                        else
                        {
                            bstatus=ButtonStatus::normal;
                            update=1;
                        }
                    }
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            {
                if(Point(e.button.x,e.button.y).inRect(r))
                {
                    if(bstatus!=ButtonStatus::clicked)
                    {
                        if(onStatusChanged)
                            onStatusChanged(running,update,bstatus,ButtonStatus::clicked);
                        else
                        {
                            bstatus=ButtonStatus::clicked;
                            update=1;
                        }
                    }
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            {
                if(Point(e.button.x,e.button.y).inRect(r))
                {
                    if(bstatus!=ButtonStatus::mouseover)
                    {
                        if(onStatusChanged)
                            onStatusChanged(running,update,bstatus,ButtonStatus::mouseover);
                        else
                        {
                            bstatus=ButtonStatus::mouseover;
                            update=1;
                        }
                    }
                }
            }
            break;
        }
    }
private:
    int disid,upid;
    Rect r;
    /// normal
    /// mouseover(MouseOver) -> clicked(Clicked) -> mouseover(ButtonUp)
    RGBA normal,mouseover,clicked;


    /// Status 0:Deactivate 1:MouseOver 2:Clicked
    ButtonStatus bstatus;
};

int main()
{
    SDLSystem::Init();
    Window wnd("Title Is Here",1024,768);
    Renderer rnd=wnd.getRenderer();

    SimpleButton button(Rect(0,0,150,150));
    button.onStatusChanged=[](int& r,int& u,SimpleButton::ButtonStatus& s,SimpleButton::ButtonStatus ns)
    {
        printf("Status From %d to %d\n",static_cast<int>(s),static_cast<int>(ns));
        if(s!=ns) u=1;
        s=ns;
    };
    button.setNormalColor(RGBA(255,0,0,255));
    button.setMouseoverColor(RGBA(0,255,0,255));
    button.setClickedColor(RGBA(0,0,255,255));
    button.enable();
    EventLoop(rnd);


    SDLSystem::Quit();
    return 0;
}
