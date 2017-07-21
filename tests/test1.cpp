#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "MiniEngine.h"
#include "MiniEngine_Event.h"
using namespace MiniEngine;

int main()
{
    try
    {
        /// Initialize SDL2, SDL2_image, SDL2_mixer, SDL2_ttf with default flags.
        SDLSystem sys;
        /// Create window
        Window wnd("Window Title",1024,768);
        /// Create renderer
        Renderer rnd(wnd);
        /// Load texture
        Texture t=rnd.loadTexture("test.png");
        /// Create a event loop
        Looper lp;
        /// Add event handling functions.
        lp.add(SDL_QUIT,[](Looper& lp){lp.stop();});
        /// Set Updater
        lp.updater=[&]()
        {
            /// Clear screen
            rnd.clear();
            /// Copy the texture to full-fill the screen.
            rnd.copyFullFill(t);
            /// Update screen
            rnd.update();
        };
        /// Start Looper
        lp.run();

        /// Texture will be freed.
        /// Renderer will be destroyed.
        /// Window will be destroyed.

        /// All SDL system will be shut down.
    }
    catch (const ErrorViewer& e)
    {
        printf("Exception: %s\n",e.what());
    }
    return 0;
}
