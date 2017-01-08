#include "App.h"

using namespace Engine;
using namespace MiniEngine;

namespace App
{
    /// Application Main Method
    void Main()
    {
        Window wnd(1024,768);
        Renderer rnd=wnd.getRenderer();

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
