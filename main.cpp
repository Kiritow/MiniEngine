#include "config.h"
#include "App.h"

InitManager_SDL syssdl;
InitManager_IMG sysimg;
InitManager_TTF systtf;
InitManager_Mix sysmix;

int main()
{
    mlog_init();
    /// Create The Default Window Instance
    WindowManager::getDefaultInstance();
    App::Main();
    WindowManager::releaseDefaultInstance();
    return 0;
}
