#include "WindowManager.h"

WindowManager* default_WindowManager=nullptr;

struct WindowManager::impl
{
    SDL_Renderer* rnd;
    SDL_Window* wnd;
    int winw,winh;
};

WindowManager::WindowManager()
{
    pimpl=new impl;
    Init();
}

WindowManager* WindowManager::getDefaultInstance() /// static
{
    if(default_WindowManager) return default_WindowManager;
    default_WindowManager=new WindowManager;
    return default_WindowManager;
}
int WindowManager::releaseDefaultInstance() /// static
{
    if(!default_WindowManager) return -1;
    delete default_WindowManager;
    default_WindowManager=nullptr;
    return 0;
}

void WindowManager::Init()
{
    mlog("Called WindowSizeManager::Init()");
    if(pimpl->wnd!=NULL) return;
    pimpl->wnd=SDL_CreateWindow("MiniEngine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1024,768,SDL_WINDOW_SHOWN);
    updateSize();
    pimpl->rnd=SDL_CreateRenderer(pimpl->wnd,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
}

void WindowManager::Quit()
{
    mlog("Called WindowSizeManager::Quit()");
    if(pimpl->rnd==NULL) return;

    SDL_DestroyRenderer(pimpl->rnd);
    pimpl->rnd=nullptr;
    SDL_DestroyWindow(pimpl->wnd);
    pimpl->wnd=nullptr;

    pimpl->winw=pimpl->winh=0;
}

WindowManager::~WindowManager()
{
    Quit();
    delete pimpl;
}

int WindowManager::getwinw()
{
    return pimpl->winw;
}

int WindowManager::getwinh()
{
    return pimpl->winh;
}

SDL_Window* WindowManager::getWindow()
{
    return pimpl->wnd;
}

SDL_Renderer* WindowManager::getRender()
{
    return pimpl->rnd;
}

void WindowManager::updateSize()
{
    SDL_GetWindowSize(pimpl->wnd,&pimpl->winw,&pimpl->winh);
}
