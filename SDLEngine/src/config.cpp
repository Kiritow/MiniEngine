#include "config.h"

namespace Global
{
    void ErrorQuit(const char* ErrorMessage)
    {
        mlog(ErrorMessage);
        exit(0);
    }
}
