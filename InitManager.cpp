#include "InitManager.h"

InitManager_SDL::InitManager_SDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    {
        #ifndef __C4DROID__
        Global::ErrorQuit("Failed to init SDL2.");
        #else
        /// C4droid does not fully support SDL2,
        ///     so initializing everything crashes.
        #endif
    }
}

InitManager_SDL::~InitManager_SDL()
{
    SDL_Quit();
}

InitManager_IMG::InitManager_IMG()
{
    if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG)<0)
    {
        Global::ErrorQuit("Failed to init SDL2 Image.");
    }
}

SDL_Surface* InitManager_IMG::loadimage(const char* Filename)
{
    return IMG_Load(Filename);
}

SDL_Texture* InitManager_IMG::loadtexture(SDL_Renderer* rnd,const char* Filename)
{
    return IMG_LoadTexture(rnd,Filename);
}

InitManager_IMG::~InitManager_IMG()
{
    IMG_Quit();
}

int InitManager_TTF::ctm=0;

InitManager_TTF::InitManager_TTF()
{
    /// ~_~ check ctm and add it anyway
    if(ctm++>0)
    {
        return;
    }

    if(TTF_Init()<0)
    {
        Global::ErrorQuit("Failed to init SDL2 TTF.");
    }
    _font=NULL;
}

int InitManager_TTF::openFont(const char* FileName,int Size)
{
    _font=TTF_OpenFont(FileName,Size);
    if(_font==NULL) return -1;
    else return 0;
}

TTF_Font* InitManager_TTF::font()
{
    return _font;
}

int InitManager_TTF::closeFont()
{
    TTF_CloseFont(_font);
    _font=NULL;
    return 0;
}

InitManager_TTF::~InitManager_TTF()
{
    /// Close Font anyway.
    if(_font) closeFont();

    /// ~_~ Firstly ctm=ctm-1, if ctm still > 0, then return ( TTF_Quit Not Executed )
    if(--ctm>0)
    {
        return;
    }

    TTF_Quit();
}

InitManager_Mix::InitManager_Mix()
{
    if(Mix_Init(MIX_INIT_MP3)<0)
    {
        Global::ErrorQuit("Failed to Init Mixer.");
    }

    if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024)<0)
    {
        Global::ErrorQuit("Failed to OpenAudio");
    }
    Mix_AllocateChannels(32);
}

InitManager_Mix::~InitManager_Mix()
{
    Mix_CloseAudio();
    Mix_Quit();
}

InitManager_SDL* syssdl=NULL;
InitManager_IMG* sysimg=NULL;
InitManager_TTF* systtf=NULL;
InitManager_Mix* sysmix=NULL;
