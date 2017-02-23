#include "mini_engine.h"

void AllInit()
{
    mlog_init();

    syssdl=new InitManager_SDL;
    sysimg=new InitManager_IMG;
    systtf=new InitManager_TTF;
    sysmix=new InitManager_Mix;

}

void AllQuit()
{
    delete sysmix;
    delete systtf;
    delete sysimg;
    delete syssdl;
}
