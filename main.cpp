#include "config.h"
#include "App.h"

int main()
{
    AllInit();
    /// Create The Default Window Instance
    WindowManager::getDefaultInstance();
    App::Main();
    WindowManager::releaseDefaultInstance();
    AllQuit();
    return 0;
}
