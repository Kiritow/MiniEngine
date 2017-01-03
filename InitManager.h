#pragma once
#include "config.h"
#include <vector>

class InitManager_SDL
{
public:
    InitManager_SDL();
    ~InitManager_SDL();
};

class InitManager_TTF
{
public:
    InitManager_TTF();
    ~InitManager_TTF();
    int openFont(const char* FileName,int Size);
    int closeFont();
    TTF_Font* font();
private:
    TTF_Font* _font;
    static int ctm;
};

class InitManager_IMG
{
public:
    InitManager_IMG();
    ~InitManager_IMG();
    SDL_Surface* loadimage(const char* FileName);
    SDL_Texture* loadtexture(SDL_Renderer* rnd,const char* Filename);
};
class InitManager_Mix
{
public:
    InitManager_Mix();
    ~InitManager_Mix();
};

extern InitManager_SDL* syssdl;
extern InitManager_IMG* sysimg;
extern InitManager_TTF* systtf;
extern InitManager_Mix* sysmix;
