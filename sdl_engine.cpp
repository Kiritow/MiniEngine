#include "sdl_engine.h"
#include "unistd.h"
#include <memory>
using namespace std;

SDL_Texture* RenderText(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph)
{
    SDL_Surface* surf=TTF_RenderText_Blended(font,Text,color);
    if(surf==NULL) return NULL;
    SDL_Texture* texture=SDL_CreateTextureFromSurface(rnd,surf);
    SDL_FreeSurface(surf);
    if(pw&&ph) SDL_QueryTexture(texture,NULL,NULL,pw,ph);
    return texture;
}

SDL_Texture* RenderUTF8(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph)
{
    SDL_Surface* surf=TTF_RenderUTF8_Blended(font,Text,color);
    if(surf==NULL) return NULL;
    SDL_Texture* texture=SDL_CreateTextureFromSurface(rnd,surf);
    SDL_FreeSurface(surf);
    if(pw&&ph) SDL_QueryTexture(texture,NULL,NULL,pw,ph);
    return texture;
}

void TextureDraw(SDL_Renderer* rnd,SDL_Texture* texture,int dstx,int dsty)
{
    if(!rnd||!texture) return;
    int w,h;
    SDL_QueryTexture(texture,NULL,NULL,&w,&h);
    SDL_Rect rect;
    rect.x=dstx;
    rect.y=dsty;
    rect.w=w;
    rect.h=h;
    SDL_RenderCopy(rnd,texture,NULL,&rect);
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

SDL_Rect Rect::toSDLRect()
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;
    return rect;
}
Rect::Rect()
{
    x=y=w=h=0;
}
Rect::Rect(int incx,int incy,int incw,int inch)
{
    x=incx;
    y=incy;
    w=incw;
    h=inch;
}
Rect::~Rect()
{

}

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

Window::Window(int winw,int winh)
{
    pimpl=new impl;
    pimpl->sWnd.reset(SDL_CreateWindow("Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,winw,winh,SDL_WINDOW_SHOWN),SDL_DestroyWindow);
    pimpl->rnd.pimpl->sRnd.reset(SDL_CreateRenderer(pimpl->sWnd.get(),-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE),SDL_DestroyRenderer);
}

Window::~Window()
{
    delete pimpl;
}

Renderer Window::getRenderer()
{
    return pimpl->rnd;
}

Renderer::Renderer()
{
    pimpl=new impl;
}
Renderer::~Renderer()
{
    delete pimpl;
}
int Renderer::clear()
{
    return SDL_RenderClear(pimpl->sRnd.get());
}
Texture Renderer::loadImage(const char* FileName)
{
    Texture t;
    t.pimpl->sText.reset(IMG_LoadTexture(pimpl->sRnd.get(),FileName),SDL_DestroyTexture);
    t.updateSize();
    return t;
}
Texture Renderer::render(Surface surface)
{
    Texture t;
    t.pimpl->sText.reset(SDL_CreateTextureFromSurface(pimpl->sRnd.get(),surface.pimpl->sSurf.get()));
    t.updateSize();
    return t;
}
void Renderer::update()
{
    SDL_RenderPresent(pimpl->sRnd.get());
}
int Renderer::copy(Texture t,Rect src,Rect dst)
{
    SDL_Rect s=src.toSDLRect();
    SDL_Rect d=dst.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),&s,&d);
}
int Renderer::copyTo(Texture t,Rect dst)
{
    SDL_Rect d=dst.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),NULL,&d);
}
int Renderer::copyFill(Texture t,Rect src)
{
    SDL_Rect s=src.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),&s,NULL);
}
int Renderer::copyFullFill(Texture t)
{
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),NULL,NULL);
}

Texture::Texture()
{
    pimpl=new impl;
}
int Texture::getw()
{
    return pimpl->w;
}
int Texture::geth()
{
    return pimpl->h;
}
Texture::~Texture()
{
    delete pimpl;
}
int Texture::updateSize()
{
    return SDL_QueryTexture(pimpl->sText.get(),NULL,NULL,&pimpl->w,&pimpl->h);
}

RGBA::RGBA()
{
    r=g=b=a=0;
}
RGBA::RGBA(int incr,int incg,int incb,int inca)
{
    r=incr;
    g=incg;
    b=incb;
    a=inca;
}
SDL_Color RGBA::toSDLColor()
{
    SDL_Color color;
    color.r=r;
    color.g=g;
    color.b=b;
    color.a=a;
    return color;
}

struct Font::impl
{
    shared_ptr<TTF_Font> sTTF;
};

Font::Font()
{
    pimpl=new impl;
}

Font::Font(const char* FontFileName,int sz) : Font()
{
    use(FontFileName,sz);
}

int Font::use(const char* FontFileName,int sz)
{
    TTF_Font* font=TTF_OpenFont(FontFileName,sz);
    if(font==NULL) return -1;
    pimpl->sTTF.reset(font,TTF_CloseFont);
    return 0;
}

Font::~Font()
{
    delete pimpl;
}

Texture

}/// End of namespace Engine
