#pragma once
#include "config.h"

class WindowManager
{
public:
    WindowManager();
    ~WindowManager();
    int getwinw();
    int getwinh();
    SDL_Renderer* getRender();
    SDL_Window* getWindow();
    void updateSize();

    static WindowManager* getDefaultInstance();
    static int releaseDefaultInstance();
protected:
    void Init();
    void Quit();
private:
    struct impl;
    impl* pimpl;
};
