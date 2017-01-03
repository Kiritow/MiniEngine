#include "config.h"
#include "App.h"

int main()
{
    mlog_init();
    /// Create The Default Window Instance
    WindowManager::getDefaultInstance();
    App::Main();
    WindowManager::releaseDefaultInstance();
    return 0;
}
