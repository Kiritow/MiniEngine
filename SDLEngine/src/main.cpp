#include "config.h"
#include "App.h"

int main()
{
    /// Initialize SDL2...
    AllInit();
    /// Call Application Main
    App::Main();
    /// Clean Up SDL2
    AllQuit();
    return 0;
}
