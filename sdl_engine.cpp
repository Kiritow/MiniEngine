#include "sdl_engine.h"
#include "unistd.h"
#include <memory>
using namespace std;

SDL_Texture* RenderUTF8(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph)
{
    SDL_Surface* surf=TTF_RenderUTF8_Blended(font,Text,color);
    if(surf==NULL) return NULL;
    SDL_Texture* texture=SDL_CreateTextureFromSurface(rnd,surf);
    SDL_FreeSurface(surf);
    if(pw&&ph) SDL_QueryTexture(texture,NULL,NULL,pw,ph);
    return texture;
}

bool isInRect(int x,int y,SDL_Rect rect)
{
    return ((x>=rect.x&&x<=rect.x+rect.w)&&(y>=rect.y&&y<=rect.y+rect.h));
}

bool isInRect(int x,int y,int LU_x,int LU_y,int RD_x,int RD_y)
{
    return ((x>=LU_x&&x<=RD_x)&&(y>=LU_y&&y<=RD_y));
}

void ClearMessageQueue()
{
    /// Clear Message Queue
    while(SDL_PeepEvents(NULL,1,SDL_GETEVENT,SDL_FIRSTEVENT,SDL_LASTEVENT));
}

SDL_Color color_white { 255,255,255 };
SDL_Color color_black { 0,0,0 };

int MyChangeDir(const char* DirName)
{
    mlog("Change Dir to \"%s\"",DirName);
    int ret=chdir(DirName);
    mlog("Change Dir returns %d",ret);
    return ret;
}


namespace Engine
{

/// Rect
#include "sdl_engine_rect.hpp"

struct Window::impl
{
    shared_ptr<SDL_Window> sWnd;
    Renderer rnd;
};

struct Renderer::impl
{
    shared_ptr<SDL_Renderer> sRnd;
};

struct Texture::impl
{
    shared_ptr<SDL_Texture> sText;
    int w,h;
};

struct Surface::impl
{
    shared_ptr<SDL_Surface> sSurf;
};

struct Font::impl
{
    shared_ptr<TTF_Font> sTTF;
};

/// Window
#include "sdl_engine_window.hpp"
/// Renderer
#include "sdl_engine_renderer.hpp"
/// Surface
#include "sdl_engine_surface.hpp"
/// Texture
#include "sdl_engine_texture.hpp"
/// RGBA
#include "sdl_engine_rgba.hpp"
/// Font
#include "sdl_engine_font.hpp"



}/// End of namespace Engine
