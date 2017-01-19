#include "App.h"
#include "Game/Main.h"

using namespace Engine;
using namespace MiniEngine;

namespace App
{
    /// Application Main Method
    void Main()
    {
        Window wnd(1366,768);///16:9
        Renderer rnd=wnd.getRenderer();
        Font bigf;
        if(bigf.use("msyh.ttf",72)<0)
        {
            mlog("Failed to open Font.");
            return;
        }
        Game::Main(wnd,rnd,bigf);

        /*
        /// Sample Code of Brush
        Brush b(wnd,Rect(wnd.getSize().w/2-50,wnd.getSize().h/2-50,100,100));
        Texture t=rnd.loadImage("D:\\sample.png");
        Rect dst(0,0,wnd.getSize().w,wnd.getSize().h);
        b.copyTo(t,dst,false);
        rnd.update();
        SDL_Delay(2000);
        */
    }
}
