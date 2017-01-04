#include "App.h"

using namespace Engine;

namespace App
{
    /// Application Main Method
    void Main()
    {
        Window wnd(1024,768);
        Renderer rnd=wnd.getRenderer();
        Texture text=rnd.loadImage("sample.jpg");
        rnd.clear();
        rnd.copyFullFill(text);
        rnd.update();
        SDL_Delay(3000);
    }
}
