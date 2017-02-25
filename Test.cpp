#include "MiniEngine.h"
using namespace MiniEngine;

#include <iostream>
using namespace std;

int AppMain()
{
    Window wnd("Sample Program",1280,720);
    Renderer rnd=wnd.getRenderer();
    rnd.setBlendMode(BlendMode::Blend);
    Font font("D:\\msyh.ttf",18);

    MusicPlayer mp;
    Music mlogin=mp.loadMusic("res/1.mp3");
    Texture tlogin=rnd.loadTexture("res/page.jpg");
    tlogin.setBlendMode(BlendMode::Blend);
    Texture tlogin2=rnd.loadTexture("res/page2.jpg");
    tlogin2.setBlendMode(BlendMode::Blend);

    mp.play(mlogin,0);

    int fade=0;
    int fadex=0;
    int aa=255;
    int ba=0;

    EventHandle::RegistDispatcher(SDL_QUIT,[](SDL_Event e,int& r,int& u){r=0;u=1;});
    EventHandle::RegistDispatcher(SDL_KEYDOWN,[&](SDL_Event e,int& r,int& u){
        if(e.key.keysym.sym==SDLK_RETURN)
        {
            fade=1;
            u=1;
        }
        else if(e.key.keysym.sym==SDLK_KP_ENTER)
        {
            fadex=1;
            u=1;
        }
    });
    EventHandle::RegistUpdater([&](Renderer& rnd){
        if(fade)
        {
            cout<<"Fading..."<<endl;
            --aa;
            ++ba;
            if(aa==0)
            {
                fade=0;
                cout<<"Fade Stop"<<endl;
            }
        }

        if(fadex)
        {
            cout<<"FadingX..."<<endl;
            ++aa;
            --ba;
            if(aa==255)
            {
                fadex=0;
                cout<<"FadeX Stop"<<endl;
            }
        }
        tlogin.setAlphaMode(aa);
        rnd.copyTo(tlogin,Point(0,0));
        tlogin2.setAlphaMode(ba);
        rnd.copyTo(tlogin2,Point(0,0));
    });

    EventHandle::Loop(rnd);

    return 0;
}

